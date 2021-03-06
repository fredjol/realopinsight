/*
 * ngrt4nd-mt.cpp
# ------------------------------------------------------------------------ #
# Copyright (c) 2010-2012 Rodrigue Chakode (rodrigue.chakode@ngrt4n.com)   #
# Last Update: 24-05-2012                                                 #
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

#include "config.h"
#include "core/ns.hpp"
#include "core/MonitorBroker.hpp"
#include <cassert>
#include <stdexcept>
#include <iostream>
#include <unistd.h>
#include <zmq.hpp>
#include <crypt.h>
#include <sstream>
#include <fstream>
#include <zmq.h>

using namespace std;


pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

int numWorkers = 1 ;

string statusFile = "/usr/local/nagios/var/status.dat" ;
string authChain = "" ; // Suitably set later
string packageName = PACKAGE_NAME ;

ostringstream help(""
		"SYNOPSIS\n"
		"	" + packageName +" [OPTIONS]\n"
		"\n"
		"OPTIONS\n"
		"	-c FILE\n"
		"	 Specifies the path of the status file. Default is " + statusFile + ".\n"
		"	-D\n"
		"	 Runs ngrt4nd in the foreground. \n"
		"	-n\n"
		"	 Sets the number of threads to start. Default is 1.\n"
		"	-p\n"
		"	 Sets the port of listening. Default is 1983.\n"
		"	-P\n"
		"	 Changes the authentification passphrase.\n"
		"	-v\n"
		"	 Prints the version and license information.\n"
		"	-h\n"
		"	 Prints this help.\n") ;

void ngrt4n::setPassChain(char* authChain) {

	ofstream ofpass;

	ofpass.open( ngrt4n::AUTH_FILE.c_str() );
	if( ! ofpass.good()) {
		cerr << "Unable to set the password:  perhaps the application's settings file is not well configured." << endl;
		exit(1) ;
	}

	ofpass << crypt(authChain, salt.c_str());
	ofpass.close();
	cout << "Password reseted"<< endl ;
}

string ngrt4n::getPassChain() {

	string authChain ;
	ifstream pfile;

	pfile.open ( ngrt4n::AUTH_FILE.c_str() );
	if( ! pfile.good()) {
		cerr << "Unable to get application's settings" << endl;
		exit(1) ;
	}

	pfile >> authChain ;
	pfile.close();
	return authChain ;
}


void *worker_routine (void *arg)
{
	zmq::context_t *ctx = (zmq::context_t*) arg;
	zmq::socket_t comChannel (*ctx, ZMQ_REP);
	comChannel.connect ("inproc://ngrt4ndwkrs");

	MonitorBroker* monitor = new MonitorBroker( statusFile ) ;
	while (true) {
		zmq::message_t request;
		request.rebuild() ;
		comChannel.recv(&request) ;
		size_t msize = request.size() ;

		char* msg = (char*)malloc(msize * sizeof(char)) ;
		memcpy(msg, request.data(), msize) ;

		char* _pass = strtok(msg, ":") ;
		string pass = (_pass == NULL) ? "" : _pass ;
		char* _sid = strtok(NULL, ":") ;
		string sid = (_sid == NULL) ? "" : _sid ;

		string result = (pass == authChain )? monitor->getInfOfService(sid) : "-2#Wrong authentification" ;
		msize = result.size() ;
		zmq::message_t reply( MonitorBroker::MAX_MSG) ;
		memset(reply.data(), 0, MonitorBroker::MAX_MSG) ;
		memcpy(reply.data(), result.c_str(), msize);
		comChannel.send(reply);

		free(msg) ;
	}

	comChannel.close() ;
	return NULL ;
}

int main(int argc, char ** argv)
{
	ostringstream versionMsg;
	versionMsg<< PACKAGE_NAME<< ", version " << PACKAGE_VERSION <<  ".\n\n"
			<< "Copyright (c) 2010-2012, NGRT4N Project <contact@ngrt4n.com>.\n"
			<< "All rights reserved. Visit "<< PACKAGE_URL<< " for more information.";

	bool foreground = false;
	static const char *shotOpt="DPhvc:p:n:" ;
	int port = MonitorBroker::DEFAULT_PORT ;
	char opt ;
	while ((opt = getopt(argc, argv, shotOpt)) != -1) {
		switch (opt)
		{
		case 'D':
			foreground = true ;
			break;

		case 'c':
			statusFile = optarg ;
			break;

		case 'p': {
			port = atoi(optarg) ;
			if(port <= 0 ) {
				cerr << "ERROR: Bad port number." << endl ;
				exit(1) ;
			}
			break;
		}

		case 'n': {
			numWorkers = atoi(optarg) ;
			if(numWorkers <= 0 ) {
				cerr << "ERROR: Bad number of instances." << endl ;
				exit(1) ;
			}
			break;
		}

		case 'P': {
			ngrt4n::checkUser() ;
			ngrt4n::initApp() ;
			char* pass = getpass("Type the passphrase:");
			ngrt4n::setPassChain(pass) ;
			cout << ngrt4n::getPassChain() << endl ;
			exit(0) ;
		}

		case 'v': {
			cout << versionMsg.str() << endl ;
			exit(0) ;
		}

		case 'h': {
			cout << help.str() << endl ;
			exit(0) ;
		}
		default: {
			cerr << "Unknown option: " << opt << endl;
			cout << help.str() << endl ;
			exit(0) ;
		}
		}
	}

	ngrt4n::checkUser() ;
	ngrt4n::initApp() ;
	authChain = ngrt4n::getPassChain() ;

	if( ! foreground ) {
		pid_t pid = fork();
		if(pid <= -1) {
			cerr << "Can not fork process" << endl;
			exit(1);
		}
		else if(pid > 0) {
			exit (0);
		}
		setsid();
	}

	ostringstream tcpAddr;
	tcpAddr << "tcp://*:" << port ;

	cout << "Starting "<< versionMsg.str() << endl ;
	cout << "Listening address => " << tcpAddr.str() << endl ;
	cout << "Nagios status file => " << statusFile << endl ;

	zmq::context_t ctx(1);
	zmq::socket_t workersComChannel(ctx, ZMQ_DEALER);
	workersComChannel.bind("inproc://ngrt4ndwkrs");

	zmq::socket_t clientsComChannel(ctx, ZMQ_ROUTER);
	clientsComChannel.bind(tcpAddr.str().c_str());

	for (int i = 0; i < numWorkers; i++) {
		pthread_t worker;
		int rc = pthread_create (&worker, NULL, worker_routine, (void*) &ctx);
		assert (rc == 0);
	}

	zmq::device (ZMQ_QUEUE, clientsComChannel, workersComChannel);

	return 0;
}
