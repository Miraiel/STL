#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<fstream>
#include<map>
#include<list>
#include<time.h>
#include<string>
#include<Windows.h>

using namespace std;

#define delimetr "\n-----------------------------\n"

const std::map<int, std::string> crime =
{
	{0, "�������������� �� ���������� (��� ��� ����)"},
	{1, "���������� ��������"},
	{2, "������ �� �������"},
	{3, "������ ������������"},
	{4, "����� �� ��������� ������"},
	{5, "����������� �������"},
	{6, "���� � ��������� ���������"},
	{7, "����� �� ���������"},
	{8, "�������� � ������������ �����"},
	{9, "����������� ������� ��������"}
};

#define TAKE_TIME int min, int hour, int day, int month, int year
#define GIVE_TIME min, hour, day, month, year

class Crime
{
	static int width;
	int id;				//������
	std::string place;	//����� ��������������
	tm time;			//����� ��������������

public:

	int get_width()const
	{
		return width;
	}

	int get_id()const
	{
		return id;
	}

	const std::string get_place()const
	{
		return place;
	}

	const char* get_time()const
	{
		//time_t t_time = mktime(&time);
		return asctime(&time);
	}

	time_t get_timestamp()const
	{
		struct tm time = this->time;
		return mktime(&time);
	}

	void set_width()
	{
		for (std::map<int, std::string>::const_iterator it = crime.begin(); it != crime.end(); ++it)
		{
			if (it->second.size() > width)width = it->second.size();
		}
	}

	void set_id(int id)
	{
		if (id >= crime.size())id = 0;
		this->id = id;
	}

	void set_place(std::string place)
	{
		this->place = place;
	}

	void set_time(TAKE_TIME)
	{
		// time_t - ����� � ������� 'timestamp'
		// tm - ��������� � ������� ��������� ������� ������� (����, ������, ��� � �.�.)
		// ������� localtime() ����������� ����� �� ������� 'timestamp' � ������ 'tm'
		// ������� mktime() ����������� ����� �� ������� 'tm' � ������ 'timestapm'
		// 
		time = tm();
		time.tm_min = min;
		time.tm_hour = hour;
		time.tm_mday = day;
		time.tm_mon = month - 1;
		time.tm_year = year - 1900;
		//��������� �������������� �����, ����� ��������� ������������������� 
		//����������-���� ��������� tm_wday(���� ������), ��������� 'tm'
		//

		time_t t_time = mktime(&time);
		tm* new_time = localtime(&t_time);
		time = *new_time;
	}

	void set_time(time_t time)
	{
		tm* p_time = localtime(&time);
		this->time = *p_time;
	}

	Crime(int id, const std::string place, TAKE_TIME)
	{
		set_width();
		set_id(id);
		set_place(place);
		set_time(GIVE_TIME);
	}

	~Crime() {}

	Crime& operator()(std::string crime)
	{
		id = stoi(crime, 0, 10);			//����������� ������ ����� � ������ � ����������� ��� � id
		while (crime.front() == ' ')crime.erase(0, 1);
		crime.erase(0, crime.find(' '));	//������� ����������� ����� �� ������

		set_time(stoi(crime, 0, 10));		//������ timestamp �� ������
		crime.erase(0, 1);					//������� ������ � ������ ������
		crime.erase(0, crime.find(' '));	//������� timestamp �� ������
		while (crime.front() == ' ')crime.erase(0, 1);
		set_place(crime);
		return *this;
	}
};

int Crime::width = 1;


std::ostream& operator<<(std::ostream& os, const Crime& obj)
{
	//os << obj.get_id() << " ";
	//os << obj.get_place() << " ";
	//os << obj.get_time();
	std::string time = obj.get_time();
	time[time.size() - 1] = 0;
	//	return os << time << ": " << crime.at(obj.get_id()) << ", " << obj.get_place();
	os << time << ": ";
	os.width(obj.get_width());
	os << std::left;
	os << crime.at(obj.get_id());
	os << obj.get_place();
	return os;
}

std::ifstream& operator>>(std::ifstream& is, Crime& obj)
{
	int id;
	time_t datetime;
	std::string place;
	is >> id >> datetime;
	std::getline(is, place, ',');
	obj.set_id(id);
	obj.set_time(datetime);
	obj.set_place(place);
	return is;
}

void print(const std::map<std::string, std::list<Crime>>& base)
{
	for (std::map<std::string, std::list<Crime>>::const_iterator it = base.begin(); it != base.end(); ++it)
	{
		cout << it->first << ":\n";

		for (std::list<Crime>::const_iterator c_it = it->second.begin(); c_it != it->second.end(); ++c_it)
		{
			cout << "\t" << *c_it << endl;
		}
		cout << delimetr << endl;
	}
}

void print_number(const std::map<std::string, std::list<Crime>>& base)
{
	std::string licence_plate;
	cout << "������� ����� ����������: "; cin >> licence_plate;
	try
	{
		for (std::list<Crime>::const_iterator it = base.at(licence_plate).begin(); it != base.at(licence_plate).end(); ++it)
		{
			cout << "\t" << *it << endl;
		}
	}
	catch (const std::exception&)
	{
		std::cerr << "������ ��� � ����" << endl;
	}
	cout << endl;
}

void print_range(const std::map<std::string, std::list<Crime>>& base)
{
	std::string start_plate;
	std::string end_plate;
	cout << "������� ��������� ����� ���������: "; cin >> start_plate;
	cout << "������� �������� ����� ���������: "; cin >> end_plate;
	for (std::map<std::string, std::list<Crime>>::const_iterator it = base.lower_bound(start_plate); it != base.upper_bound(end_plate); ++it)
	{
		cout << it->first << ":\n";
		for (std::list<Crime>::const_iterator c_it = it->second.begin(); c_it != it->second.end(); ++it)
		{
			cout << "\t" << *c_it << endl;
		}
		cout << delimetr << endl;
	}
}

void add_crime(std::map<std::string, std::list<Crime>>& base)
{
	for (std::pair<int, std::string> i : crime)cout << i.first << "\t" << i.second << endl;
	int id;								//������
	std::string licence_plate;			//����� ����������
	std::string place;					//����� ���������
	int min, hour, day, month, year;	//���� � ����� ���������
	cout << "������� ����� ������: "; cin >> id;
	cout << "������� ����� ����������: "; cin >> licence_plate; cin.ignore();
	cout << "������� ����� ���������: "; std::getline(cin, place);
	SetConsoleCP(1251);
	std::getline(cin, place);
	SetConsoleCP(866);
	cout << "������� ���� � ����� ���������: "; cin >> year >> month >> day >> hour >> min;
	base[licence_plate].push_back(Crime(id, place, min, hour, day, month, year));
}

void save(const std::map<std::string, std::list<Crime>>& base, const std::string& filename)
{
	std::ofstream fout(filename);
	for (std::map<std::string, std::list<Crime>>::const_iterator it = base.begin(); it != base.end(); ++it)
	{
		fout << it->first << ":\t";
		for (std::list<Crime>::const_iterator c_it = it->second.begin(); c_it != it->second.end(); ++c_it)
		{
			fout << c_it->get_id() << " " << c_it->get_timestamp() << " ";
			//fout.seekp(-3, ios::cur);
			fout << " " << c_it->get_place() << ", ";
		}
		fout.seekp(-2, std::ios::cur);
		fout << endl;
	}
	fout.close();
	std::string command = "notepad ";
	command += filename;
	system(command.c_str());
}

void load(std::map<std::string, std::list<Crime>>& base, const std::string& filename)
{
	base.clear();
	std::ifstream fin(filename);
	if (fin.is_open())
	{
		std::string licence_plate;
		const int SIZE = 1024;
		char all_crime[SIZE] = {};
		Crime crime(0, "", 1	, 1, 1, 1, 1971);
		while (!fin.eof())
		{
			std::getline(fin, licence_plate, ':');
			if (licence_plate.empty())continue;
			//while (crime.get_place().back() != ';')
			//{
			//	fin >> crime;
			//}
			fin.getline(all_crime, SIZE, ';');
			for (char* pch = strtok(all_crime, ",;"); pch; pch = strtok(NULL, ";"))
			{
				crime(pch);
				base[licence_plate].push_back(crime);
			}
		}
		fin.ignore();
		fin.close();
	}
	else
	{
		std::cerr << "File not found" << endl;
	}
}

//#define CRIME_CHECK
//#define TIME_CHECK

void main()
{
	setlocale(LC_ALL, "");

#ifdef CRIME_CHECK
	Crime crime(10, " ��. ������ ", 23, 22, 1, 5, 2021);
	cout << crime << endl;
#endif // CRIME_CHECK

#ifdef TIME_CHECK
	cout << "������� ����� � ������� 'timestamp' :" << time(NULL) << endl;
	time_t t_current_time = time(NULL);
	tm* p_tm_curren_time = localtime(&t_current_time);
	cout << "������� ����� � ������� 'tm' \t    :" << asctime(p_tm_curren_time) << endl;
#endif // TIME_CHECK

	std::map<std::string, std::list<Crime>> base;/*=
	{
		{"m777aa",
			{
				Crime(1, "��. ������ 2", 22, 11, 29, 4, 2023),
				Crime(2, "��. ������", 23, 11, 29, 4, 2023),
				Crime(4, "��. ������", 24, 11, 29, 4, 2023)
			}
		},

		{"m001aa",
			{
				Crime(6, "��. �������", 30, 18, 30, 4, 2023),
				Crime(5, "��. �������", 40, 18, 30, 4, 2023)
			}
		},

		{"b123cc",
			{
				Crime(6, "��. �����������", 10, 8, 8, 3, 2022),
				Crime(3, "��. �����������", 22, 18, 11, 4, 2022)
			}
		}
	};*/

	int key;
	do
	{
		cout << "�������� ��������:\n";
		cout << "1. ������� ���� �� �����;" << endl;
		cout << "2. ��������� ���� � ����;" << endl;
		cout << "3. ��������� ���� �� �����;" << endl;
		cout << "4. �������� ���������;" << endl;
		cout << "5. ������� ��� ��������� ��������� ������;" << endl;
		cout << "6. ������� ��� ��������� �� ��������� �������;" << endl;
		cout << "0. �����" << endl;
		cin >> key;
		system("CLS");
		switch (key)
		{
		case 1: print(base); break;
		case 2: save(base, "base.txt"); break;
		case 3:load(base, "base.txt"); break;
		case 4: add_crime(base); break;
		case 5: print_number(base); break;
		case 6: print_range(base); break;
		}
		system("PAUSE");
		system("CLS");
	} while (key);

}





