#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <conio.h>
#include<algorithm>
using namespace std;

struct Node
{
	string info;
	int popular;
	vector<Node *> NodeChild;
};

Node *createNode(string data)
{
	Node *temp = new Node;
	temp->info = data;
	temp->popular = 0;
	return temp;
}

void createTrie(Node *&dad, string text)
{
	if (dad->NodeChild.empty())
	{
		Node *temp = createNode(text);
		dad->NodeChild.push_back(temp);
		return;
	}
	for (auto &nodePtr : dad->NodeChild)
	{
		for (int i = nodePtr->info.length() - 1; i >= 0; i--)
		{
			if (text.find(nodePtr->info.substr(0, 1 + i)) == 0)
			{
				// Sau khi tim thay doan dau cua Text va NodePtr giong nhau, thi tach doan do ra
				if (i == (nodePtr->info.length() - 1))
				{
					nodePtr->popular++;
					if (text.substr(i + 1) == "")
					{
						return;
					}
					return createTrie(nodePtr, text.substr(i + 1));
				}
				else
				{
					// Cat phan giong nhau cua nodePtr ra dua vao vecto
					Node *temp = createNode(nodePtr->info.substr(i + 1));
					temp->NodeChild = nodePtr->NodeChild;
					nodePtr->NodeChild.clear();
					nodePtr->NodeChild.push_back(temp);
					nodePtr->popular++;
					nodePtr->info = nodePtr->info.substr(0, i + 1);
					// Cat phan giong nhau cua text roi dua vao nodePtr
					if (text.substr(i + 1) == "")
					{
						return;
					}
					return createTrie(nodePtr, text.substr(i + 1));
				}
			}
		}
	}
	Node *temp = createNode(text);
	dad->NodeChild.push_back(temp);
}

void add(Node *&root, string text)
{
	int length = text.length() - 1;
	for (int i = 0; i <= length; i++)
	{
		string temp = text.substr(length - i);
		createTrie(root, temp);
	}
}

void xuat(Node *dad)
{
	for (Node *ptr : dad->NodeChild)
	{
		cout << ptr->info << ", " << ptr->popular << endl;
		cout << "=====" << endl;
		xuat(ptr);
		cout << "------------------------------" << endl;
	}
}

void clearScreen()
{
#ifdef _WIN32
	system("cls");
#else
	// Trường hợp hệ điều hành khác, có thểa sử dụng các phương pháp khác tùy thuộc vào hệ điều hành
	system("clear");
#endif
}

void printTrie(Node *node, string s)
{
	if (node){
		int dem=0;string d;
		if(dem==0){
			dem++;
		 d= s.substr(1,s.length());
		
		}
		else if(dem!=0){
		 d= s.substr(0,s.length()-1);
		}
		if(node->popular==0){
        cout << d << node->info << endl;
		}

		}
		for (const auto child : node->NodeChild)
		{
			printTrie(child, s + node->info );
		}
}
void test(){}

void suggestWords(Node *&root, string suggestion)
{
	Node *node = root;
	// Tim nut cuoi cung cua tu goi y
	for (char ch : suggestion)
	{
		bool found = false;
		for (auto &child : node->NodeChild)
		{
			if (child->info.find(ch)==0)
			{
				node = child;
				found = true;
				break;
			}
		}
		if (!found)
		{
			cout << "_found_nothing_" << endl;
			return;
		}
	}
	sort(node->NodeChild.begin(), node->NodeChild.end(), [](Node *a, Node *b) {
		return a->popular > b->popular;
	});
	
	
	string s = suggestion;
	printTrie(node, s);
	cout << endl;
}

void LongestCommonTrieString(Node *dad, int thanhphan, string a, vector<string> &temp)
{
	for (Node *ptr : dad->NodeChild)
	{
		if (ptr->popular > 0)
		{
			temp.push_back(a + ptr->info + to_string(thanhphan));
			LongestCommonTrieString(ptr, thanhphan + 1, a + ptr->info, temp);
		}
	}
}

void LongestCommonTrieString(Node *dad)
{
	string a = "";
	int thanhphan = 1, b;
	vector<string> temp;
	LongestCommonTrieString(dad, thanhphan, a, temp);
	for (string ptr : temp)
	{
		b = stoi(ptr.substr(ptr.length() - 1));
		if (b > thanhphan)
		{
			thanhphan = b;
		}
	}
	for (string ptr : temp)
	{
		cout << ptr << " ";
	}
	cout << endl;
	cout << "Chuoi dai nhat: " << endl;
	for (string ptr : temp)
	{
		b = stoi(ptr.substr(ptr.length() - 1));
		if (b == thanhphan)
		{
			cout << ptr.substr(0, ptr.length() - 1) << endl;
		}
	}
}
void menu()
{
	string text,  temp = "";string suggestion;
	int a = 0;
	char ch;
	Node *root = createNode("root");
	ifstream input;
	input.open("word.txt", ios::in);
	while (!input.eof())
	{
		getline(input, text);
		add(root, text);
	}
	int lt;
	cout << "====================MENU====================\n";
	cout << "1. Search keyword (mimic GG)\n";
	cout << "2. Print Suffix Tree\n";
	cout << "3. Find Longest Common Substring\n";
	cout << "============================================\n";
	do
	{
		cout << "_Action: ";
		cin >> lt;
		switch (lt)
		{
		case 1:
			cout << "Nhap tu goi y: ";
			ch = _getch();
			while (ch != '$')
			{

				suggestion += ch;
				clearScreen();
				suggestWords(root, suggestion);
				cout << "Nhap tu goi y: " << suggestion;
				ch = _getch();
			}
			cout<<suggestion;
			break;
		case 2:
			xuat(root);
			break;
		case 3:
			LongestCommonTrieString(root);
			break;
		}

	} while (lt != 0);
}
int main()
{
	menu();
	return 0;
}