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
  puts(msg.toStdString().c_str());

  return true;
}

void document_server::helloWorld()
{
  QTcpSocket *clientCon = server_socket->nextPendingConnection();
  cl_object findNs = makefn("qmapper.root:root-findNs");
  cl_object saveNs = makefn("qmapper.root:root-saveNs");
  cl_object is_glsl = makefn("qmapper.root:is-ns-glsl?");
  cl_object is_lisp = makefn("qmapper.root:is-ns-lisp?");

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
						  std::string ns = "\""+qns.replace(QString("\n"), QString("")).replace(QString("\r"), QString("")).toStdString()+"\"";
						  printf("finding ns %s\n", ns.c_str());
						  std::string contents(ecl_string_to_string(get(cl_funcall(3, findNs,
												       editorController::instance->document.getValue(),
													   c_string_to_object(ns.c_str())), "contents")));
						  printf("contents: %s\n", contents.c_str());
						  QString c(contents.c_str());

						  QString firstLine = c.split(QString("\n")).at(0);
						  bool this_lisp = cl_funcall(3, is_lisp,
										editorController::instance->document.getValue(),
										c_string_to_object(ns.c_str())) == ECL_T;
						  
						  QString result = firstLine.contains(QRegExp("mode: (lisp|glsl)")) ? c:
						    (this_lisp? QString("; -*- mode: lisp; -*-\n"):
						     QString("// -*- mode: glsl; -*-\n")) + c;
	    


						  qDebug() << "Found script " << c;
						  printf("script result is %s\n", result.toStdString().c_str());

						  clientCon->write(result.toUtf8());
						}
						else if (protocol == QString("SAVE-NS")) {
						  std::string ns = l.at(1).toStdString();
						  std::string contents = str.replace(l.at(0)+":"+l.at(1)+":", QString("")).toStdString();
						  qDebug() << "Saving to NS " << ns.c_str() << " contents: " << contents.c_str();
	  
						  editorController::instance->document.setValue(cl_funcall(4, saveNs,
													   editorController::instance->document.getValue(),
													   c_string_to_object(("\""+ns+"\"").c_str()),
													   c_string_to_object(("\""+contents+"\"").c_str())));
						}
						else qDebug() << "Didn't recognize protocol " << protocol;
					      }
					      else qDebug() << "No protocol part found in message " << str;
					      clientCon->disconnectFromHost();
      
					    });
  
  connect(clientCon, &QAbstractSocket::disconnected,
	  clientCon, &QObject::deleteLater);
}
