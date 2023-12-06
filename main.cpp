#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <conio.h>
#include <algorithm>
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

void printTrie(Node *root, const string typing, int &dem)
{
	if (root)
	{
		string d;
		if (dem == 0)
		{
			dem++;
			// Remove the first character only once
			d = typing.substr(1, typing.length());
		}
		else
		{
			// Remove the last character in subsequent calls
			d = typing.substr(0, typing.length() - 1);
		}
		if (root->popular == 0)
		{
			cout << d << root->info << endl;
		}
	}
	for (const auto child : root->NodeChild)
	{
		// Append the current node's info to the string for recursive calls
		printTrie(child, typing + root->info, dem);
	}
}
void suggestWords(Node *dad, string suggestion, string cat, vector<string> &temp)
{

	for (Node *child : dad->NodeChild)
	{
		if (suggestion == "")
		{
			temp.push_back(cat + child->info);
			suggestWords(child, suggestion, cat + child->info, temp);
		}
		else
		{
			for (int i = child->info.length() - 1; i >= 0; i--)
			{
				if (child->info < suggestion)
				{
					if (suggestion.find(child->info.substr(0, 1 + i)) == 0)
					{
						if (suggestion.substr(i + 1) == "")
						{
							suggestion = "";
							temp.push_back(cat + child->info);
							return suggestWords(child, suggestion, cat + child->info, temp);
						}
						else
						{
							suggestWords(child, suggestion.substr(i + 1), cat + child->info, temp);
						}
					}
				}
				else
				{
					if (child->info.find(suggestion) == 0)
					{
						suggestion = "";
						temp.push_back(cat + child->info);
						return suggestWords(child, suggestion, cat + child->info, temp);
					}
				}
			}
		}
	}
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
	/*for (string ptr : temp)
	{
		cout << ptr << " ";
	}*/
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
	string text, temp = "", cat = "";
	string suggestion;
	vector<string> tempp;

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
	cout << "====================MENU====================\n";
	cout << "1. Search keyword (mimic GG)\n";
	cout << "2. Print Suffix Tree\n";
	cout << "3. Find Longest Common Substring\n";
	cout << "0. Escape\n";
	cout << "============================================\n";
	int action;
	do
	{
		cout << "_Action: ";
		cin >> action;
		switch (action)
		{
		case 1:
			cout << "Nhap tu goi y: ";
			ch = _getch();
			while (ch != '$')
			{
				if (ch == 8)
					suggestion = suggestion.substr(0, suggestion.length() - 1);
				else
					suggestion += ch;
				clearScreen();
				cout << "Type this character "
						" $ "
						" to stop searching\n";
				cout << "-----Nhap tu goi y: " << suggestion << endl;
				if (suggestion != "")
					suggestWords(root, suggestion, cat, tempp);
				if (tempp.empty())
				{
					cout << "Found Nothing" << endl;
				}
				else
				{
					for (string a : tempp)
					{
						cout << a << endl;
					}
				}
				ch = _getch();
				tempp.clear();
			}
			clearScreen();
			cout << "====================MENU====================\n";
			cout << "1. Search keyword (mimic GG)\n";
			cout << "2. Print Suffix Tree\n";
			cout << "3. Find Longest Common Substring\n";
			cout << "0. Escape\n";
			cout << "============================================\n";
			break;
		case 2:
			xuat(root);
			break;
		case 3:
			LongestCommonTrieString(root);
			break;
		}

	} while (action != 0);
}
int main()
{
	menu();
	return 0;
}