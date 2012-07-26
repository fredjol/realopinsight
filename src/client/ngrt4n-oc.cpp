/*
 * ngrt4n.cpp
# ------------------------------------------------------------------------ #
# Copyright (c) 2010-2012 Rodrigue Chakode (rodrigue.chakode@ngrt4n.com)   #
# Last Update : 24-05-2012                                                 #
#                                                                          #
# This file is part of NGRT4N (http://ngrt4n.com).                         #
#                                                                          #
# NGRT4N is free software: you can redistribute it and/or modify           #
# it under the terms of the GNU General Public License as published by     #
# the Free Software Foundation, either version 3 of the License, or        #
# (at your option) any later version.                                      #
#                                                                          #
# NGRT4N is distributed in the hope that it will be useful,                #
# but WITHOUT ANY WARRANTY; without even the implied warranty of           #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            #
# GNU General Public License for more details.                             #
#                                                                          #
# You should have received a copy of the GNU General Public License        #
# along with NGRT4N.  If not, see <http://www.gnu.org/licenses/>.          #
#--------------------------------------------------------------------------#
 */

#include "core/ns.hpp"
#include "client/Auth.hpp"
#include "client/SvNavigator.hpp"
#include "client/SvConfigCreator.hpp"
#include <sstream>
#include <getopt.h>

QString cmdName = "" ;
QString  usage = "usage: " + cmdName + " [OPTION] [view_config]\n"
        "Options: \n"
        "	-c\n"
        "	   Launch the configuration utility\n"
        "	-v\n"
        "	  Print the version and license information.\n"
        "	-h \n"
        "	   Print this help.\n" ;


ostringstream versionMsg(appName.toStdString() + " Operations Console, Version " + packageVersion.toStdString() + ".\n\n"
		+"Copyright (c) 2010-"+releaseYear.toStdString()+", NGRT4N Project <contact@ngrt4n.com>.\n"
		+"All rights reserved. Visit "+packageUrl.toStdString()+" for further information.");

int main(int argc, char **argv)
{
    QApplication* app = new QApplication(argc, argv) ;
    app->setWindowIcon(QIcon (":images/built-in/icon.png")) ;
    app->setApplicationName(appName) ;
    app->setStyleSheet(Preferences::style());
    cmdName=argv[0];

    if(argc > 3) {
        qDebug() << usage ;
        exit (1) ;
    }

    bool config = false ;
    QString file = argv[1] ;
    int opt ;

    if ( (opt = getopt(argc, argv, "chv") ) != -1) {
        switch (opt) {
        case 'c':
            config = true ;
            break ;

        case 'v': {
            cout << versionMsg.str() << endl;
            exit(0) ;
        }

        case 'h': {
            cout << usage.toStdString() ;
            exit(0) ;
        }

        default:
            cout << "Syntax Error :: " << usage.toStdString() ;
            exit (1) ;
            break ;
        }
    }
	cout <<"Launching "<<versionMsg.str()<<endl;
    Auth authentication;
    int userRole = authentication.exec() ;
    if( userRole != Auth::ADM_USER_ROLE && userRole != Auth::OP_USER_ROLE ) exit(1) ;

    if( config ) {
        Preferences* update_settings = new Preferences(userRole, Preferences::ChangeMonitoringSettings) ;
        Preferences* change_passwd = new Preferences(userRole, Preferences::ChangePassword) ;
        update_settings->exec() ;
        change_passwd->exec() ;
        exit(0) ;
    }

    QSplashScreen* info = Preferences::infoScreen("Welcome to "+QString::fromStdString(versionMsg.str()));
    sleep(2);
    if(file == "") {
        info->clearMessage();
        info->showMessage("You need to select a configuration file!", Qt::AlignCenter|Qt::AlignCenter);
        sleep(1); info->finish(0);
        file = QFileDialog::getOpenFileName(0,
                                            appName.toUpper()+" :: Select a configuration file",
                                            ".",
                                            "Xml files (*.xml);;All files (*)");

        if(! file.length()){
            QMessageBox::critical(0,
                                  appName.toUpper() + " :: Info",
                                  "No configuration file has been selected and the program will exit.",
                                  QMessageBox::Ok);
            exit (1) ;
        }
    }
    info->finish(0);
    SvNavigator *monitor= new SvNavigator(userRole, file) ; monitor->startMonitor() ;

    return app->exec() ;
}