#include "connections_settingstab.h"

#include "appservers.h"

ConnectionsSettingsTab::ConnectionsSettingsTab(QWidget *parent) : QWidget(parent) {
  buildUi();
  wireUi();
}

ConnectionsSettingsTab::~ConnectionsSettingsTab() {
  delete serversList;
  delete connectionEditArea;

  delete gridLayout;
}

void ConnectionsSettingsTab::buildUi() {
  gridLayout = new QGridLayout(this);

  connectionEditArea = new ConnectionProperties(this);
  connectionEditArea->setEnabled(false); // start disabled, since no item is selected

  serversList = new ServersList(this);

  // Layout
  /*        0                 1
       +---------------+--------------+
    0  |  Servers List |  Connection  |
       |               |  Properties  |
       +---------------+--------------+
  */

  // row 0
  gridLayout->addWidget(serversList, 0, 0);
  gridLayout->addWidget(connectionEditArea, 0, 1);

  gridLayout->setColumnStretch(0, 1);
  gridLayout->setColumnStretch(1, 3);

  this->setLayout(gridLayout);
}

void ConnectionsSettingsTab::wireUi() {
  connect(connectionEditArea, &ConnectionProperties::onSave, this, &ConnectionsSettingsTab::onConnectionSaved);
  connect(serversList, &ServersList::onServerSelectionChanged, this, &ConnectionsSettingsTab::serverSelectionChanged);

  connect(serversList, &ServersList::onServerAdded, [this](){ connectionEditArea->highlightNameTextbox(); });
}

void ConnectionsSettingsTab::setMargin(int width) {
  gridLayout->setMargin(width);
}

void ConnectionsSettingsTab::onConnectionSaved(ServerItem item) {
  AppServers::getInstance().updateServer(item);
}

void ConnectionsSettingsTab::serverSelectionChanged(std::vector<ServerItem> selectedServers) {
  connectionEditArea->setEnabled(selectedServers.size() == 1);
  if (selectedServers.size() == 0) {
    connectionEditArea->clearForm();
    return;
  }
  connectionEditArea->loadItem(selectedServers[0]);
}

void ConnectionsSettingsTab::resetForm() {
  serversList->clearSelection();
  serversList->refreshList();
}