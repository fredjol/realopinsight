/*
# ------------------------------------------------------------------------ #
# Copyright (c) 2015 Rodrigue Chakode (rodrigue.chakode@gmail.com)         #
# Last Change: 17-12-2017                                                  #
#                                                                          #
# This file is part of RealOpInsight (http://RealOpInsight.com) authored   #
# by Rodrigue Chakode <rodrigue.chakode@gmail.com>                         #
#                                                                          #
# RealOpInsight is free software: you can redistribute it and/or modify    #
# it under the terms of the GNU General Public License as published by     #
# the Free Software Foundation, either version 3 of the License, or        #
# (at your option) any later version.                                      #
#                                                                          #
# The Software is distributed in the hope that it will be useful,          #
# but WITHOUT ANY WARRANTY; without even the implied warranty of           #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            #
# GNU General Public License for more details.                             #
#                                                                          #
# You should have received a copy of the GNU General Public License        #
# along with RealOpInsight.  If not, see <http://www.gnu.org/licenses/>.   #
#--------------------------------------------------------------------------#
 */

#ifndef WEBBASESETTINGS_HPP
#define WEBBASESETTINGS_HPP

#include "BaseSettings.hpp"
#include "dbo/src/DbSession.hpp"
#include <Wt/WCheckBox>


const std::string LDAP_VERSION2_LABEL = "LDAPv2";
const std::string LDAP_VERSION3_LABEL = "LDAPv3";

class WebBaseSettings : public BaseSettings
{
public:
  enum AuthenticationModeT {
    BuiltIn = 0,
    LDAP = 1
  };

  enum NotificationTypeT {
    NoNotification = 0,
    EmailNotification = 1
  };

  WebBaseSettings(void);
  int getDbType(void) const;
  std::string getDbServerAddr(void) const;
  int getDbServerPort(void) const;
  std::string getDbName(void) const;
  std::string getDbUser(void) const;
  std::string getDbPassword(void) const;
  std::string getDbConnectionString(void) const;

  std::string getLdapServerUri(void) const { return m_settingFactory->keyValue(SettingFactory::AUTH_LDAP_SERVER_URI).toStdString();}
  std::string getLdapBindUserDn(void) const { return m_settingFactory->keyValue(SettingFactory::AUTH_LDAP_BIND_USER_DN).toStdString();}
  std::string getLdapSearchBase(void) const { return m_settingFactory->keyValue(SettingFactory::AUTH_LDAP_SEARCH_BASE).toStdString();}
  std::string getLdapBindUserPassword(void) const { return m_settingFactory->keyValue(SettingFactory::AUTH_LDAP_BIND_USER_PASSWORD).toStdString();}
  std::string getLdapIdField(void) const;
  int getLdapVersion(void) const;
  bool getLdapSslUseMyCert(void) const {return m_settingFactory->keyValue(SettingFactory::AUTH_LDAP_SSL_USE_CERT).toInt() == Wt::Checked;}
  std::string getLdapSslCertFile(void) const {return m_settingFactory->keyValue(SettingFactory::AUTH_LDAP_SSL_CERT_FILE).toStdString();}
  std::string getLdapSslCaFile(void) const {return m_settingFactory->keyValue(SettingFactory::AUTH_LDAP_SSL_CA_FILE).toStdString();}

  int getAuthenticationMode(void) const;
  static std::string authTypeString(int authSystem) {return (authSystem == LDAP) ? "LDAP" : "Built-in";}
  int getNotificationType(void) const { return m_settingFactory->keyValue(SettingFactory::NOTIF_TYPE).toInt();}

  std::string getSmtpServerAddr(void) const { return m_settingFactory->keyValue(SettingFactory::NOTIF_MAIL_SMTP_SERVER_ADRR).toStdString();}
  std::string getSmtpServerPortText(void) const { return m_settingFactory->keyValue(SettingFactory::NOTIF_MAIL_SMTP_SERVER_PORT).toStdString();}
  int getSmtpServerPort(void) const { return m_settingFactory->keyValue(SettingFactory::NOTIF_MAIL_SMTP_SERVER_PORT).toInt();}
  std::string getSmtpUsername(void) const { return m_settingFactory->keyValue(SettingFactory::NOTIF_MAIL_SMTP_USERNAME).toStdString();}
  std::string getSmtpPassword(void) const { return m_settingFactory->keyValue(SettingFactory::NOTIF_MAIL_SMTP_PASSWORD).toStdString();}
  int getSmtpUseSsl(void) const { return m_settingFactory->keyValue(SettingFactory::NOTIF_MAIL_SMTP_USE_SSL).toInt();}

protected :
  virtual void fillInFormGivenSourceId(int sid) {}
  virtual void updateAllSourceWidgetStates(void) {}
  virtual void updateFields(void) {}

protected Q_SLOTS:
  virtual void applyChanges(void) {}
  virtual void handleCancel(void) {}
  virtual void addAsSource(void) {}
  virtual void deleteSource(void){}
};
#endif // WEBBASESETTINGS_HPP
