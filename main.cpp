#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <conio.h>
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

void creatTrie(Node *&dad, string text)
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
					return creatTrie(nodePtr, text.substr(i + 1));
				}
				else
				{
					// Cat phan giong nhau cua nodePtr ra dua vao vecto
					Node *temp = createNode(nodePtr->info.substr(i + 1));
					temp->NodeChild = nodePtr->NodeChild;
					nodePtr->NodeChild.clear();
					nodePtr->NodeChild.push_back(temp);
					nodePtr->popular++;
					// Cat phan giong nhau cua text roi dua vao nodePtr
					if (text.substr(i + 1) == "")
					{
						return;
					}
					temp = createNode(text.substr(i + 1));
					nodePtr->NodeChild.push_back(temp);
					nodePtr->info = nodePtr->info.substr(0, i + 1);
					return;
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
		creatTrie(root, temp);
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
	// Trường hợp hệ điều hành khác, có thể sử dụng các phương pháp khác tùy thuộc vào hệ điều hành
	system("clear");
#endif
}

void printTrie(Node *node, string s)
{
	if (node)
	{
		string d = s.substr(0, s.length() - 1);
		cout << d << node->info << endl;

		for (const auto child : node->NodeChild)
		{
			printTrie(child, s + node->info);
		}
	}
}

void suggestwords(Node *root, string suggestion)
{
	Node *node = root;
	// Tim nut cuoi cung cua tu goi y
	for (char ch : suggestion)
	{
		bool found = false;
		for (auto &child : node->NodeChild)
		{
			if (child->info[0] == ch)
			{
				node = child;
				found = true;
				break;
			}
		}
		if (!found)
		{
			cout << "/////// " << endl;
			return;
		}
	}

	cout << " ";
	string s = suggestion;
	printTrie(node, s);
	cout << endl;
}

void longestcommonTrieString(Node *dad, int thanhphan, string a, vector<string> &temp)
{
	for (Node *ptr : dad->NodeChild)
	{
		if (ptr->popular > 0)
		{
			temp.push_back(a + ptr->info + to_string(thanhphan));
			longestcommonTrieString(ptr, thanhphan + 1, a + ptr->info, temp);
		}
	}
}

void longestcommonTrieString(Node *dad)
{
	string a = "";
	int thanhphan = 1, b;
	vector<string> temp;
	longestcommonTrieString(dad, thanhphan, a, temp);
	for (string ptr : temp)
	{
		b = stoi(ptr.substr(ptr.length() - 1));
		if (b > thanhphan)
		{
			thanhphan = b;
		}
	}

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
	string text, suggestion, temp = "";
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
	do
	{
		cout << "Nhap lt: ";
		cin >> lt;
		switch (lt)
		{
		case 1:
			suggestion;
			cout << "Nhap tu goi y: ";
			ch = _getch();
			while (ch != '$')
			{
				suggestion += ch;
				clearScreen();
				suggestwords(root, suggestion);

				cout << "Nhap tu goi y: " << suggestion;
				ch = _getch();
			}
			break;
		case 2:
			xuat(root);
			break;
		case 3:
			longestcommonTrieString(root);
			break;
		}

	} while (lt != 0);
}
int main()
{
	menu();
	return 0;
}