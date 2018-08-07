#include <doc-server.h>
#include <QNetworkInterface>
#include <editorController.h>
#include <QRegExp>
#include <guile_fn.h>

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
  static cl_object findNs = ecl_make_symbol("root-findNs", "CL-USER");
  static cl_object saveNs = ecl_make_symbol("root-saveNs", "CL-USER");
  static cl_object is_glsl = ecl_make_symbol("is-ns-glsl?", "CL-USER");
  static cl_object is_scheme = ecl_make_symbol("is-ns-scheme?", "CL-USER");

  connect(clientCon, &QIODevice::readyRead, [=]() {
					      QByteArray block;
					      // QDataStream out(&block, QIODevice::WriteOnly);
					      // out.setVersion(QDataStream::Qt_4_0);

					      QByteArray array = clientCon->readAll();
					      QString str(array);

					      QStringList l = str.split(":");
					      if(l.size() > 0) {
						QString protocol = l.at(0);

						if(protocol == QString("NS")) {
						  QString qns = l.at(1);
						  std::string ns = qns.replace(QString("\n"), QString("")).replace(QString("\r"), QString("")).toStdString();

						  std::string contents(ecl_string_to_string(cl_funcall(3, findNs,
												       editorController::instance->document,
												       c_string_to_object(ns.c_str()))));
						  QString c(contents.c_str());

						  QString firstLine = c.split(QString("\n")).at(0);
						  bool this_scheme = cl_funcall(3, is_scheme,
										editorController::instance->document,
										c_string_to_object(ns.c_str())) == ECL_T;
						  
						  // either<scriptTypes, std::string> script_data = editorController::instance->document.findNs(ns);
						  QString result = firstLine.contains(QRegExp("mode: (scheme|glsl)")) ? c:
						    (this_scheme? QString("; -*- mode: scheme; -*-\n"):
						     QString("// -*- mode: glsl; -*-\n")) + c;
	    


						  qDebug() << "Found script " << c;

						  clientCon->write(result.toUtf8());
						}
						else if (protocol == QString("SAVE-NS")) {
						  std::string ns = l.at(1).toStdString();
						  std::string contents = str.replace(l.at(0)+":"+l.at(1)+":", QString("")).toStdString();
						  qDebug() << "Saving to NS " << ns.c_str() << " contents: " << contents.c_str();
	  
						  editorController::instance->document = cl_funcall(4, saveNs,
												    editorController::instance->document,
												    c_string_to_object(ns.c_str()),
												    c_string_to_object(contents.c_str()));
						}
						else qDebug() << "Didn't recognize protocol " << protocol;
					      }
					      else qDebug() << "No protocol part found in message " << str;
					      clientCon->disconnectFromHost();
      
					    });
  
  connect(clientCon, &QAbstractSocket::disconnected,
	  clientCon, &QObject::deleteLater);
}
