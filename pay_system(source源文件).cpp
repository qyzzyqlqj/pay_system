//C:\Users\Public\Documents
#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#include <cstdio>
#include <iostream>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <string>
using namespace std;
string password = "";
ofstream fout;
ifstream fin;
string passwordfile = "C:/Users/Public/Documents/pay_system_password.txt";
string logsfile = "C:/Users/Public/Documents/pay_system_logs.txt";
const char* repasswordfile = "C:/Users/Public/Documents/pay_system_password.txt";
const char* relogsfile = "C:/Users/Public/Documents/pay_system_logs.txt";
string hashget(const string& data) {
	EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
	if (!mdctx) {
		cerr << "Error creating EVP_MD_CTX." << endl;
		return "";
	}
	if (!EVP_DigestInit_ex(mdctx, EVP_sha256(), nullptr)) {
		cerr << "Error initializing SHA256 context." << endl;
		EVP_MD_CTX_free(mdctx);
		return "";
	}
	if (!EVP_DigestUpdate(mdctx, data.c_str(), data.length())) {
		cerr << "Error updating hash with data." << endl;
		EVP_MD_CTX_free(mdctx);
		return "";
	}
	unsigned char hash[EVP_MAX_MD_SIZE];
	unsigned int hashLen;
	if (!EVP_DigestFinal_ex(mdctx, hash, &hashLen)) {
		cerr << "Error finalizing hash." << endl;
		EVP_MD_CTX_free(mdctx);
		return "";
	}
	stringstream ss;
	for (unsigned int i = 0; i < hashLen; i++) {
		ss << hex << setw(2) << setfill('0') << static_cast<int>(hash[i]);
	}
	EVP_MD_CTX_free(mdctx);

	return ss.str();
}
bool passcheck() {
	string passin;
	cout << "enter the password" << endl;
	for (int i = 0; i < 3; i++) {
		printf("you have %d times:->", 3 - i);
		getline(cin, passin);
		if (hashget(passin) == password) {
			return true;
		}
		cout << "password is false!" << endl;
	}
	return false;
}
struct info {
	string date = "";
	string way = "";
	string num = "";
};
string getCurrentDateTime() {
	time_t now = time(nullptr);
	tm* localTime = localtime(&now);
	ostringstream oss;
	oss << (localTime->tm_year + 1900) << '/'
		<< setw(2) << setfill('0') << (localTime->tm_mon + 1) << '/'
		<< setw(2) << setfill('0') << localTime->tm_mday << '.'
		<< setw(2) << setfill('0') << localTime->tm_hour << ':'
		<< setw(2) << setfill('0') << localTime->tm_min << ':'
		<< setw(2) << setfill('0') << localTime->tm_sec;
	return oss.str();
}
void log() {
	cout << "enter 'infos' to look infos" << endl << "enter 'del' to del the last one of your infos(you need to have infos)" << endl;
	cout << "enter 'num+' to money in and 'num-' to money out" << endl << "enter 'exit' to exit " << endl;
	cout << "enter 'recover' to recover the program *#(tips:everything will back)" << endl;
	cout << "enter 'pwdchange' to change the password" << endl;
}
void recoverey() {
	if (remove(relogsfile)) {
		cout << "error!!!can't remove files,please go to \"C:/Users/Public/Documents/\" to delete 'pay_system_logs.txt'" << endl;
	}
	if (remove(repasswordfile)) {
		cout << "error!!!can't remove files,please go to \"C:/Users/Public/Documents/\" to delete 'pay_system_password.txt'" << endl;
	}
	cout << "done! need to restart!" << endl;
	cout << "system closing..." << endl << "done..." << endl;
	return;
}
int main() {
	vector<info>ans;
	int cnt = 0, total = 0;
	printf("welcome to use pay_system!\n");
	cout << endl;
	fin.open(passwordfile);
	if (!fin.good()) {
		fin.close();
		fout.open(passwordfile);
		string pin1, pin2;
		while (true) {
			cout << "please create the password:->";
			cin >> pin1;
			cout << "be sure about you password,enter again:->";
			cin >> pin2;
			if (pin1 != pin2) {
				cout << "not same,creat again!" << endl;
				continue;
			}
			break;
		}
		fout << hashget(pin1);
		password = hashget(pin1);
		fout.close();
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	else {
		getline(fin, password);
		fin.close();
		if (!passcheck()) {
			cout << "if you forget the password,enter 'y' to recover the system,else enter other:->";
			string r;
			getline(cin, r);
			if (r == "y") {
				string choose = "";
				while (choose != "y" && choose != "n") {
					cout << "check again,you are recover the system!are you sure?,everything will be deleted!!!(y/n):->";
					getline(cin, choose);
					if (choose.empty())continue;
				}
				if (choose == "y") {
					recoverey();
					fout.close();
					fin.close();
					getchar();
					return 0;
				}
			}
			cout << "system closing..." << endl << "done..." << endl;
			fout.close();
			fin.close();
			getchar();
			return 0;
		}
	}
	fin.open(logsfile);
	if (!fin.good()) {
		fin.close();
		fout.open(logsfile);
		fout.close();
	}
	else {
		string line;
		ans.clear();
		int pos = 1;
		info loadinfos;
		while (getline(fin, line)) {
			if (pos % 3 == 1)loadinfos.way = line;
			else if (pos % 3 == 2)loadinfos.date = line;
			else {
				loadinfos.num = line;
				if (loadinfos.way == "push_out")total -= stoi(loadinfos.num);
				else total += stoi(loadinfos.num);
				cnt++;
				ans.push_back(loadinfos);
			}
			pos++;
		}
	}
	fin.close();
	cout << endl;
	log();
	cout << "enter 'sys?' to see how to use" << endl << endl;
	while (true) {
		string temp = "";
		cout << ":pay_sys->";
		getline(cin, temp);
		if (temp.empty())continue;
		if (temp == "sys?") {
			log();
			cout << endl;
			continue;
		}
		if (temp == "exit") {
			string choose = "";
			while (choose != "y" && choose != "n") {
				cout << "are you sure?(y/n):->";
				getline(cin, choose);
				if (choose.empty())continue;
			}
			if (choose == "y")break;
			continue;
		}
		if (temp == "recover") {
			if (!passcheck()) {
				cout << "system closing..." << endl << "done..." << endl;
				fout.close();
				fin.close();
				getchar();
				return 0;
			}
			string choose = "";
			while (choose != "y" && choose != "n") {
				cout << "are you sure?,everything will be deleted!!!(y/n):->";
				getline(cin, choose);
				if (choose.empty())continue;
			}
			if (choose == "y") {
				string choose = "";
				while (choose != "y" && choose != "n") {
					cout << "check again,you are recover the system!are you sure?,everything will be deleted!!!(y/n):->";
					getline(cin, choose);
					if (choose.empty())continue;
				}
				if (choose == "y") {
					recoverey();
					fout.close();
					fin.close();
					getchar();
					return 0;
				}
				continue;
			}
		}
		if (temp == "pwdchange") {
			string pin1, pin2;
			if (!passcheck()) {
				cout << "system closing..." << endl << "done..." << endl;
				fout.close();
				fin.close();
				getchar();
				return 0;
			}
			while (true) {
				cout << "enter the new password:->";
				cin >> pin1;
				cout << "be sure about you new password,enter again:->";
				cin >> pin2;
				if (pin1 != pin2) {
					cout << "not same,change again!" << endl;
					continue;
				}
				break;
			}
			fout.open(passwordfile);
			fout << hashget(pin1);
			fout.close();
			cout << "done! need to restart!" << endl;
			cout << "system closing..." << endl << "done..." << endl;
			fout.close();
			fin.close();
			getchar();
			return 0;
		}
		if (temp == "infos") {
			if (!passcheck()) {
				cout << "system closing..." << endl << "done..." << endl;
				fout.close();
				fin.close();
				getchar();
				return 0;
			}
			cout << endl;
			if (!cnt) {
				cout << "no infos!" << endl;
				cout << "total get-> " << total << endl << endl;
				continue;
			}
			for (int i = 1; i <= cnt; i++)cout << i << "." << '[' << ans[i - 1].date << ']' << " <" << ans[i - 1].way << "> " << ans[i - 1].num << endl;
			cout << "total get-> " << total << endl << endl;
			continue;
		}
		if (temp == "del") {
			if (!passcheck()) {
				cout << "system closing..." << endl << "done..." << endl;
				fout.close();
				fin.close();
				getchar();
				return 0;
			}
			if (cnt == 0) {
				cout << "no infos to delete! the list is empty!!!" << endl << endl;
				continue;
			}
			fout.open(logsfile);
			if (!fout.good()) {
				cout << "can't open file,save false,things lost!!!";
				cout << "system closing..." << endl << "done..." << endl;
				fout.close();
				fin.close();
				getchar();
				return 0;
			}
			ans[cnt - 1].date = "";
			if (ans[cnt - 1].way == "push out")total += stoi(ans[cnt - 1].num);
			else total -= stoi(ans[cnt - 1].num);
			ans[cnt - 1].way = "";
			ans[cnt - 1].num = "";
			cnt--;
			for (int i = 0; i < cnt; i++) {
				fout << ans[i].way << endl;
				fout << ans[i].date << endl;
				fout << ans[i].num << endl;
			}
			fout.close();
			cout << "Done!" << endl << endl;
			continue;
		}
		else {
			bool flag = false;
			if (temp.size() == 1) {
				cout << '"' << temp << '"' << " is not defined!" << endl << endl;
				continue;
			}
			for (int i = 0; i < temp.size() - 2; i++)
				if (temp[i] < '0' || temp[i]>'9')flag = true;
			if (temp[temp.size() - 1] != '+' && temp[temp.size() - 1] != '-')
				flag = true;
			if (flag) {
				cout << '"' << temp << '"' << " is not defined!" << endl << endl;
				continue;
			}
			string currentDateTime = getCurrentDateTime();
			info newInfo;
			if (temp[temp.size() - 1] == '+') {
				newInfo.way = "pull_in";
				newInfo.date = currentDateTime;
				newInfo.num = temp.substr(0, temp.length() - 1);
				total += stoi(newInfo.num);
			}
			else if (temp[temp.size() - 1] == '-') {
				newInfo.way = "push_out";
				newInfo.date = currentDateTime;
				newInfo.num = temp.substr(0, temp.length() - 1);
				total -= stoi(newInfo.num);
			}
			ans.push_back(newInfo);
			fout.open(logsfile, ios::app);
			if (!fout.good()) {
				cout << "can't open file,save false,things lost!!!";
				cout << "system closing..." << endl << "done..." << endl;
				fout.close();
				fin.close();
				getchar();
				return 0;
			}
			fout << newInfo.way << endl;
			fout << newInfo.date << endl;
			fout << newInfo.num << endl;
			fout.close();
			cnt++;
		}
	}
	cout << endl << "thanks to use pay_system!" << endl;
	fout.close();
	fin.close();
	getchar();
	return 0;
}
