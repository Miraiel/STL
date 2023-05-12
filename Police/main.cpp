#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<map>
#include<list>
#include<time.h>

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

	void set_plase(std::string place)
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

	Crime(int id, const std::string place, TAKE_TIME)
	{
		set_width();
		set_id(id);
		set_plase(place);
		set_time(GIVE_TIME);
	}

	~Crime() {}

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

	std::map<std::string, std::list<Crime>> base =
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
	};

	for (std::map<std::string, std::list<Crime>>::iterator it = base.begin(); it != base.end(); ++it)
	{
		cout << it->first << ":\n";

		for (std::list<Crime>::iterator c_it = it->second.begin(); c_it != it->second.end(); ++c_it)
		{
			cout << "\t" << *c_it << endl;
		}
		cout << delimetr << endl;
	}
}



