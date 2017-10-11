#include <doc-server.h>
#include <QNetworkInterface>
#include <editorController.h>

document_server::document_server() {
  // Init tcp server and pair it to the sessionOpened() - slot
  // According to this http://doc.qt.io/qt-5/qtnetwork-fortuneserver-example.html

  server_socket = new QTcpServer(this);
  initialized = false;
}

bool document_server::start_server() {
  if(!server_socket->listen()) {
    initialized = false;
    QMessageBox::critical(nullptr, "QMapper",
			  QString("Unable to start server: %1").arg(server_socket->errorString()));
    return false;
  }
  else initialized = true;

  connect(server_socket, &QTcpServer::newConnection, this, &document_server::helloWorld);

  QString ipAddress;
  QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
  // use the first non-localhost IPv4 address
  for (int i = 0; i < ipAddressesList.size(); ++i) {
    if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
	ipAddressesList.at(i).toIPv4Address()) {
      ipAddress = ipAddressesList.at(i).toString();
      break;
    }
  }
  // if we did not find one, use IPv4 localhost
  if (ipAddress.isEmpty())
    ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
  
  QString msg = QString("The server is running on\n\nIP: %1\nport: %2")
                   .arg(ipAddress).arg(server_socket->serverPort());
  QMessageBox::information(nullptr, "QMapper", msg);
  qDebug () << msg;

  return true;
}

void document_server::helloWorld()
{
  QTcpSocket *clientCon = server_socket->nextPendingConnection();

  connect(clientCon, &QIODevice::readyRead, [=]() {
      QByteArray block;
      // QDataStream out(&block, QIODevice::WriteOnly);
      // out.setVersion(QDataStream::Qt_4_0);

      QByteArray array = clientCon->readAll();
      QString str(array);
      str = str.replace(QString("\n"), QString("")).replace(QString("\r"), QString(""));
      std::string ns = str.toStdString();
      
      std::string contents = editorController::instance->document.findNs(ns);
      QString c(contents.c_str());

      qDebug() << "Found script " << c;

      // out << c;

      clientCon->write(c.toUtf8());
      clientCon->disconnectFromHost();
    });
  
  connect(clientCon, &QAbstractSocket::disconnected,
	  clientCon, &QObject::deleteLater);
}
