// Copyright 2020, Verizon Media
// Licensed under the terms of MIT. See LICENSE file in project root for terms.

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

void handleCLI(std::vector<std::string> args);

#ifndef QT_NO_SYSTEMTRAYICON

#include <QApplication>
#include <QMessageBox>

#include "appconfig.h"
#include "appsettings.h"
#include "db/databaseconnection.h"
#include "exceptions/databaseerr.h"
#include "exceptions/fileerror.h"
#include "traymanager.h"
#include "models/type_streams.h"
#include "migrations/migration.h"

void initResources() {
  Q_INIT_RESOURCE(res_icons);
  Q_INIT_RESOURCE(res_migrations);
}

void setApplicationAttributes() {
  QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
  QCoreApplication::setApplicationName("ashirt");
}

DatabaseConnection* createDBConnection() {
  DatabaseConnection* conn;
  try {
    conn = new DatabaseConnection();
    conn->connect();
    return conn;
  }
  catch (FileError& err) {
    std::cout << err.what() << std::endl;
  }
  catch (DBDriverUnavailableError& err) {
    std::cout << err.what() << std::endl;
  }
  catch (QSqlError& e) {
    std::cout << e.text().toStdString() << std::endl;
  }
  catch (std::exception& e) {
    std::cout << "Unexpected error: " << e.what() << std::endl;
  }
  return nullptr;
}

DatabaseConnection* readySupportSystems() {
  // load settings
  // nothing to do here -- just make sure it's loaded
  // (technically unnecessary, but effectively zero cost)
  AppSettings::getInstance();

  // load config
  auto configError = AppConfig::getInstance().errorText().toStdString();
  if (!configError.empty()) {  // quick check & preload config data
    throw std::runtime_error("Unable to create a load configuration file: " + configError);
  }

  // start database connection (+ implicitly migrate data)
  DatabaseConnection* conn = createDBConnection();
  if (conn == nullptr) {
    throw std::runtime_error("Unable to create a database connection");
  }

  // apply System migrations
  Migration::applyMigrations(conn);

  // Upgrade Settings
  AppSettings::getInstance().upgrade();

  }

  return conn;
}

int guiMain(int argc, char* argv[]) {
  initResources();
  setApplicationAttributes();
  TypeStreams::registerTypes();
  DatabaseConnection* conn = readySupportSystems();

  int exitCode;
  try {
    QApplication app(argc, argv);

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
      handleCLI(std::vector<std::string>(argv, argv + argc));
    }
    QApplication::setQuitOnLastWindowClosed(false);

    auto window = new TrayManager(conn);
    exitCode = app.exec();
    AppSettings::getInstance().sync();
    delete window;
  }
  catch (std::exception const& ex) {
    std::cout << "Exception while running: " << ex.what() << std::endl;
  }
  catch (...) {
    std::cout << "Unhandled exception while running" << std::endl;
  }
  conn->close();
  delete conn;

  return exitCode;
}

int main(int argc, char* argv[]) {
  try {
    int ec = guiMain(argc, argv);
    if( ec != 0) {
      std::cout << "exiting with code: " << ec << std::endl;
    }
  }
  catch(const std::exception& e) {
    std::cerr << "Unhandled exception while running gui. Message: " << e.what() << " (exiting)" << std::endl;
  }
}

#else

#include <QDebug>
#include <QLabel>

int main(int argc, char *argv[]) { handleCLI(std::vector<string>(argv, argv + argc)); }

#endif

void handleCLI(std::vector<std::string> args) {
  Q_UNUSED(args);
  std::cout << "CLI is currently not supported.\n";
}
