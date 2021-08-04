#include<iostream>
#include<string>
#include<fstream>
using namespace std;

struct score     //定义存放学生成绩信息的结点
{
	int num;      //学号 
	string name;   //姓名 
	float math;   //数学成绩 
	float english;  //英语成绩
	float computer;   //计算机基础成绩 
	float scoresum;   //三门成绩总和 
	struct score * next;  //next为指向下一结点的指针 
};
struct score * head;  //指向链表头结点的指针
int studentSum = 0;  //学生总人数

class record
{
public:
	struct score * InsertRecord(struct score *h);//插入学生成绩信息 
	struct score * DeleteRecord(struct score *h);//删除学生成绩信息 
	struct score * UpdateRecord(struct score *h);//修改学生成绩信息 
	void FindRecord(struct score *h, int  x, float s1, float s2);//根据某门课程的分数段查询学生成绩信息 
	void FindRecord(struct score *h, string  x); //根据学生姓名查询学生成绩信息 
	void FindRecord(struct score *h, int x);     //根据学生学号查询学生成绩信息 
	void StatisticRecord(struct score *h, int x);  //统计某门课程的及格学生人数、及格率，并显示不及格学生信息 
	void StacRecordFind(struct score *h);    //统计三门课程成绩全部优秀的学生人数，并显示全部优秀的学生信息
	void StacRecordDisq(struct score *h);    //统计三门课程成绩全部不及格的学生人数，并显示全部不及格的学生信息 
	void PrintRecord(struct score *h);       //输出所有 学生成绩 
	void SaveRecordFile(struct score *h);     //保存学生成绩信息到文件 
	struct score *LoadRecordFile(struct score *h);   //从文件中加载学生成绩信息 
};

struct score *record:: InsertRecord(struct score *h)
{
	struct score *p1, *p2, *p3;
	p3 = new score;     //创建新的学生成绩结点
	cout << "\n请输入学生学号：";
	cin >> p3->num;
	cout << "\n请输入学生姓名：";
	cin >> p3->name;
	cout << "\n请输入学生的数学成绩：";
	cin >> p3->math;
	cout << "\n请输入学生的英语成绩：";
	cin >> p3->english;
	cout << "\n请输入学生的计算机基础成绩：";
	cin >> p3->computer;
	p3->scoresum = p3->math + p3->english + p3->computer;  //计算结点的总成绩
	p3->next = NULL;   //将要插入结点的指针域置为空

	if (h == NULL)
	{
		h = p3;
		return h;
	}
	p1 = p2 = h;
	while (p1 != NULL && p3->num > p1->num)   //查找结点的学号大于要插入结点的学号的第一个结点    
	{                   //指针p1表示符合条件的结点的指针，指针p2是指针p1的前一个结点的指针
		p2 = p1;
		p1 = p1->next;
	}
	if (p1 == h)  //插入位置为链表头结点前
	{
		p3->next = h;
		h = p3;
		return h;
	}
	else   //插入位置为链表的中间的链表的尾部
	{
		p2->next = p3;
		p3->next = p1;
	}
	studentSum += 1;  //学生人数加1
	return h;    //返回链表的头结点
}

void record::PrintRecord(score *h)
{
	if (h == NULL)
	{
		cout << "\n抱歉，没有任何记录！";
		return;
	}
	cout << "\n学号\t姓名\t数学\t英语\t计算机\t总分" << endl;
	while (h)   //输出链表中每个结点的学生成绩信息
	{
		cout << h->num << "\t" << h->name << "\t" << h->math << "\t" << h->english << "\t" << h->computer << "\t" << h->scoresum << endl;
		h = h->next;
	}
}

struct score *record::DeleteRecord(struct score *h)
{
	struct score *p1, *p2;
	int num;
	if (h == NULL)  //链表为空
	{
		cout << "\n抱歉，没有任何记录！";
		return h;
	}
	p1 = p2 = h;//将链表的头指针赋给指针p1和p2
	cout << "\n请输入要删除记录的学生学号";
	cin >> num;
	while (p1 != NULL && p1->num != num)//查找结点的学号等于要删除学生学号的第一个结点
	{                     //指针p1表示符合条件的结点的指针，指针p2是指针p1的前一个结点指针
		p2 = p1;
		p1 = p1->next;
	}
	if (p1 == NULL)
	{
		cout << "\n抱歉啊，表中没有该记录哦！";
		return h;
	}
	if (p1->num == num)
	{
		studentSum -= 1;//学生人数减1
		if (p1 == h)  //删除的是头结点
			h = h->next;
		else      //删除的是非头结点
			p2->next = p1->next;
		delete p1;  //释放p1所指向的存储单元
	}
	return h;
}

struct score *record::UpdateRecord(struct score *h)
{
	struct score *p1;
	int num;
	if (h == NULL)
	{
		cout << "\n抱歉，没有任何记录！";
		return h;
	}
	p1 = h;
	cout << "\n请输入要修改记录的学生学号：";
	cin >> num;
	while (p1 != NULL && p1->num != num) //查找结点的学号等于要修改学生学号的结点指针
	{
		p1 = p1->next;  //将p1指针移到下一个结点
	}
	if (p1 == NULL)  //没有找到符合要求的结点
	{
		cout << "\n抱歉，表中没有该记录！";
		return h;
	}
	if (p1->num == num)  //找到符合要求的结点，并修改学生的相关成绩
	{
		cout << "\n请重新输入学生的数学成绩：";
		cin >> p1->math;
		cout << "\n请重新输入学生的英语成绩:";
		cin >> p1->english;
		cout << "\n请重新输入学生的计算机基础成绩：";
		cin >> p1->computer;
		p1->scoresum = p1->math + p1->english + p1->computer;
	}
	return h;
}

void record::FindRecord(struct score *h, int x, float s1, float s2)
{
	if (h == NULL) //链表为空
	{
		cout << "\n,抱歉，没有任何记录！";
		return;
	}
	cout << "\n学号\t姓名\t数学\t英语\t计算机\t总分" << endl;
	while (h)
	{
		if (x == 1)  //查找数学成绩在某分数段的学生成绩信息
		{
			if (h->math >= s1 && h->math <= s2)
				cout << h->num << "\t" << h->name << "\t" << h->math << "\t" << h->math << "\t" << h->english << "\t" << h->computer << "\t" << h->scoresum << endl;
		}
		if (x == 2)
		{
			if (h->english >= s1 && h->english <= s2)
				cout << h->num << "\t" << h->name << "\t" << h->math << "\t" << h->math << "\t" << h->english << "\t" << h->computer << "\t" << h->scoresum << endl;

		}
		if (x == 3)
		{
			if (h->computer >= s1 && h->computer <= s2)
				cout << h->num << "\t" << h->name << "\t" << h->math << "\t" << h->math << "\t" << h->english << "\t" << h->computer << "\t" << h->scoresum << endl;
		}
		h = h->next;
	}

}

void record::FindRecord(struct score *h, int num)  //根据学生学号查找学生成绩信息
{
	struct score *p1;
	if (h == NULL)
	{
		cout << "\n抱歉，没有任何记录";
		return;
	}
	p1 = h;   //将链表的头结点指针h赋给指针p1
	while (p1 != NULL && p1->num != num) //查找结点的学号等于要查找学生学号的结点指针
	{
		p1 = p1->next;
	}
	if (p1 == NULL)   //没有找到
	{
		cout << "抱歉啊，表中没有该记录的哦！";
		return;
	}
	if (p1->num == num)  //找到并显示信息
	{
		cout << "\n学号\t姓名\t数学\t英语\t计算机\t总分" << endl;
		cout << p1->num << "\t" << p1->name << "\t" << p1->math << "\t" << p1->math << "\t" << p1->english << "\t" << p1->computer << "\t" << p1->scoresum << endl;
	}

}

void record::FindRecord(struct score *h, string name)  //根据学生姓名查找学生成绩信息
{
	struct score *p1;
	if (h == NULL)
	{
		cout << "\n抱歉，没有任何记录！";
		return;
	}
	p1 = h;
	while (p1 != NULL && p1->name != name) //查找结点的姓名等于要查找学生姓名的结点指针
	{
		p1 = p1->next;
	}
	if (p1 == NULL)
	{
		cout << "\n抱歉，表中没有该记录！";
		return;
	}
	if (p1->name == name)
	{
		cout << "\n学号\t姓名\t数学\t英语\t计算机\t总分" << endl;
		cout << p1->num << "\t" << p1->name << "\t" << p1->math << "\t" << p1->math << "\t" << p1->english << "\t" << p1->computer << "\t" << p1->scoresum << endl;
	}
}

void record::StatisticRecord(struct score* h, int x)
{
	struct score *p = h;  //将链表的头结点指针赋给指针p
	int count = 0;//定义统计人数count变量并赋初值为0
	if (p == NULL)
	{
		cout << "\n抱歉，没有任何记录！";
		return;
	}
	while (p)
	{
		if (x == 1)
			if (p->math >= 60)
				count += 1;
		if (x == 2)
			if (p->english >= 60)
				count += 1;
		if (x == 3)
			if (p->computer >= 60)
				count += 1;
		p = p->next;
	}
	if (x == 1)
	{
		cout << "数学成绩及格人数为：";
		cout << count;
		cout << ",及格率为：";
		cout << count / (float)studentSum << endl;
		if (count < studentSum)
			cout << "\n学号\t姓名\t数学" << endl;
		else
			cout << "没有数学成绩不及格学生" << endl;
	}
	else
	{
		if (x == 2)
		{
			cout << "英语成绩及格人数为：";
			cout << count;
			cout << ",及格率为：";
			cout << count / (float)studentSum << endl;
			if (count < studentSum)
				cout << "\n学号\t姓名\t英语" << endl;
			else
				cout << "没有英语成绩不及格学生" << endl;
		}
		else
		{
			if (x == 3)
			{
				cout << "计算机成绩及格人数为：";
				cout << count;
				cout << ",及格率为：";
				cout << count / (float)studentSum << endl;
				if (count < studentSum)
					cout << "\n学号\t姓名\t计算机" << endl;
				else
					cout << "没有计算机成绩不及格学生" << endl;
			}
		}

	}

	p = h;
	while (p)
	{
		if (x == 1)
			if (p->math < 60)
				cout << p->num << "\t" << p->name << "\t" << p->math << endl;
		if (x == 2)
			if (p->english < 60)
				cout << p->num << "\t" << p->name << "\t" << p->english << endl;
		if (x == 13)
			if (p->computer < 60)
				cout << p->num << "\t" << p->name << "\t" << p->computer << endl;
		p = p->next;
	}

}

void record::StacRecordFind(struct score *h)
{
	struct score *p = h;
	int count = 0;
	if (p == NULL)
	{
		cout << "\n抱歉，没有任何记录！";
		return;
	}
	while (p)
	{
		if (p->math >= 90 && p->english >= 90 && p->computer >= 90)
		{
			count += 1;
			
		}
		p = p->next;

	}
	cout << "三门成绩全为优秀的学生人数为：";
	cout << count << endl;
	cout << "全为优秀的学生信息为：" << endl;
	cout << "\n学号\t姓名\t数学\t英语\t计算机\t总分" << endl;
	p = h;
	while (p)
	{
		if (p->math >= 90 && p->english >= 90 && p->computer >= 90)
			cout << p->num << "\t" << p->name << "\t" <<p->math<<"\t"<<p->english<<"\t"<< p->computer <<"\t"<<p->scoresum<< endl;
		p = p->next;
	}
}

void record::StacRecordDisq(struct score *h)
{
	struct  score *p = h;
	int count = 0;
	if (p == NULL)
	{
		cout << "\n抱歉，没有任何记录！";
		return;
	}
	while (p)
	{
		if (p->math < 60 && p->english < 60 && p->computer < 60)
			count += 1;
		p = p->next;
	}
	cout << "三门成绩全不及格的学生人数为：";
	cout << count << endl;
	cout << "全为不及格的学生信息为：" << endl;
	cout << "\n学号\t姓名\t数学\t英语\t计算机\t总分" << endl;
	p = h;
	while (p)
	{
		if (p->math <60 && p->english <60 && p->computer <60)
			cout << p->num << "\t" << p->name << "\t" << p->math << "\t" << p->english << "\t" << p->computer << "\t" << p->scoresum << endl;
		p = p->next;
	}
}

void record::SaveRecordFile(struct score *h) //将链表中的数据写入文件
{
	struct score *p;
	ofstream ofile;  //定义输出文件对象
	ofile.open("score.dat", ios::out);  //以定的方式打开文件score.dat,若该文件不存在，则创建score.dat文件
	if (!ofile)
	{
		cout << "\n数据文件打开错误没有将数据写入文件！\n";
		return;
	}
	ofile << "\n学号\t姓名\t数学\t英语\t计算机\t总分";
	while (h)
	{
		ofile<<endl<< h->num << "\t" << h->name << "\t" << "\t" << h->math << "\t" << h->english << "\t" << h->computer << "\t" << h->scoresum;
		p = h;  //将当前结点的数据信息写入到文件中
		h = h->next;
		delete p;
	}
	ofile.close();
}

struct score* record::LoadRecordFile(struct score *h)
{
	ifstream ifile;  //定义输入文件对象
	ifile.open("score.dat", ios::in);//以读写方式打开文件score.dat
	struct score *p, *q;
	if (!ifile)
	{
		cout << "\n数据文件不存在，加载不成功！\n";
		return NULL;
	}
	char s[50];
	ifile.getline(s, 50); //读取文件指针当前行数据
	while (!ifile.eof()) 
	{
		studentSum = studentSum + 1;
		p = new score;
		ifile >> p->num >> p->name >> p->math >> p->english >> p->computer >> p->scoresum; //
		p->next = NULL;
		if (h == NULL)
		{
			q = h = p;
		}
		else
		{
			q->next = p;
			q = p;
		}
	}
	ifile.close();
	return h;
}

void SystemMenu(record r)
{
	int choice;
	while (1)
	{
		cout << "\n\t\t欢迎进入学生成绩管理系统!";
		cout << "\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";
		cout << "\n\t1、添加学生成绩信息";
		cout << "\n\t2、删除学生成绩信息";
		cout << "\n\t3、修改学生成绩信息";
		cout << "\n\t4、查询学生成绩信息";
		cout << "\n\t5、显示所有学生成绩信息";
		cout << "\n\t6、统计学生成绩信息";
		cout << "\n\t0、退出系统";
		cout << "\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";

		cout << "\n请根据提示选择操作：";
		cin >> choice;
		switch (choice)
		{
		case 1:           //增加学生成绩信息
			head = r.InsertRecord(head);
			break;
		case 2:          //删除学生成绩信息
			head = r.DeleteRecord(head);
			break;
		case 3:    //修改学生成绩信息
			head = r.UpdateRecord(head);
			break;
		case 4:    //查询活到成绩信息        
			while (1)
			{
				int c;
				cout << "\n*************************************";
				cout << "\n\t1、根据学号查询学生成绩信息";
				cout << "\n\t2、根据姓名查询学生成绩信息";
				cout << "\n\t3、根据数学分数查询学生成绩信息";
				cout << "\n\t4、根据英语分数查询学生成绩信息";
				cout << "\n\t5、根据计算机基础成绩查询学生成绩信息";
				cout << "\n\t6、返回上级目录";
				cout << "\n*************************************";

				cout << "\n请根据提示选择操作：";   //显示查询子菜单
				cin >> c;
				if (c == 1)   //根据学生学号查询学生成绩信息
				{
					int x;
					cout << "\n请输入需要查询的学生学号：";
					cin >> x;
					r.FindRecord(head, x);
				}
				if (c == 2)   //根据学生姓名查询学生成绩信息
				{
					string name;
					cout << "\n请输入需要查询的学生姓名：";
					cin >> name;
					r.FindRecord(head, name);
				}
				if (c == 3)  //根据数学分数段查询学生成绩信息
				{  
					float s1, s2;
					cout << "\n请输入查询的数学最低分的最高分：";
					cin >> s1 >> s2;
					r.FindRecord(head, 1, s1, s2);
				}
				if (c == 4)   //根据英语分数段查询学生成绩信息
				{
					float s1, s2;
					cout << "\n请输入查询的英语最低分的最高分：";
					cin >> s1 >> s2;
					r.FindRecord(head, 2, s1, s2);
				}
				if (c == 5)   //根据计算机分数段查询学生成绩信息
				{
					float s1, s2;
					cout << "\n请输入查询的计算机基础最低分的最高分：";
					cin >> s1 >> s2;
					r.FindRecord(head, 3, s1, s2);
				}
				if (c == 6)  //退出查询子菜单
					break;
			}
			break;
		case 5:           //输出所有学生成绩信息
			r.PrintRecord(head); 
			break;
		case 6:             //统计学生成绩信息
			while (1)
			{
				int c;
				cout << "\n***********************************************************";
				cout << "\n\t1、统计数学成绩及格学生人数，并显示不及格学生信息";
				cout << "\n\t2、统计英语成绩及格学生人数，并显示不及格学生信息";
				cout << "\n\t3、统计计算机成绩及格学生人数，并显示不及格学生信息";
				cout << "\n\t4、统计三门功课都不及格的学生人数，并显示学生信息（〉＝90）";
				cout << "\n\t5、统计三门功课都优秀的学生人数，并显示学生信息";
				cout << "\n\t6、返回上级目录";
				cout << "\n***********************************************************";

				cout << "\n请根据提示选择操作：";   //显示统计子菜单
				cin >> c;
				if (c == 1)
				{
					r.StatisticRecord(head, 1);//统计数学成绩及格人数，并显示不及格学生信息
				}
				if (c == 2)
				{
					r.StatisticRecord(head, 2);//统计英语成绩及格人数，并显示不及格学生信息
				}
				if (c == 3)
				{
					r.StatisticRecord(head, 3);//统计计算机成绩及格人数，并显示不及格学生信息
				}
				if (c == 4)
				{
					r.StacRecordFind(head);//统计三门功课都不及格学生人数，并显示学生信息
				}
				if (c == 5)
				{
					r.StacRecordDisq(head);//统计三门功课都优秀学生人数，并显示学生信息
				}
				if (c == 6)  //退出统计子菜单
					break;
			}
			break;
		}
		if (choice == 0)//退出系统
		break;
	}
	
}

int main()
{
	head = NULL;
	record r;                   //定义record类的对象r
	head = r.LoadRecordFile(head);  //将文件中的数据读取到链表中
	SystemMenu(r);           //显示系统菜单，并处理用户选择
	r.SaveRecordFile(head);  //将链表中的数据写到文件中
	return 0;

}

