#ifndef CONNECTIONSSETTINGSTAB_H
#define CONNECTIONSSETTINGSTAB_H

#include <QWidget>
#include <QGridLayout>


#include "components/servers_editor/connection_properties.h"
#include "components/servers_editor/servers_list.h"


class ConnectionsSettingsTab : public QWidget
{
  Q_OBJECT
 public:
  explicit ConnectionsSettingsTab(QWidget *parent = nullptr);
  ~ConnectionsSettingsTab();

 private:
  /// buildUi creates the window structure.
  void buildUi();
  /// wireUi connects the components to each other.
  void wireUi();

 private slots:
  void onConnectionSaved(ServerItem data);
  void serverSelectionChanged(std::vector<ServerItem> selectedServers);

 public:
  void setMargin(int width);
  void resetForm();

 private:
  QGridLayout* gridLayout;

  ConnectionProperties* connectionEditArea = nullptr;
  ServersList* serversList = nullptr;
};

#endif // CONNECTIONSSETTINGSTAB_H
