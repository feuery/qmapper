#ifndef DOCSERVER_H
#define DOCSERVER_H

#include<QTcpServer>
#include<QObject>
#include<QMessageBox>
#include<QTcpSocket>

class document_server: public QObject {
  Q_OBJECT
public:
  document_server();

  bool start_server();

  bool initialized;

private slots:
  // void sessionOpened();
  void helloWorld();


private:
  QTcpServer *server_socket;

};

#endif //DOC-SERVER_H
