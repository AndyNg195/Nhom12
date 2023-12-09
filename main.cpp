#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <conio.h>
#include <algorithm>
using namespace std;

struct SuffixTreeNode
{
	string info;
	int popular;
	vector<SuffixTreeNode *> NodeChild;
};
typedef SuffixTreeNode Node;

Node *createNode(string data)// hàm tạo node
{
	Node *temp = new Node;
	temp->info = data;
	temp->popular = 0;
	return temp;
}

void createSuffix(Node *&dad, string text)
{
    if (dad->NodeChild.empty())
    {
        // Điều kiện dừng của đệ quy: Nếu dad->NodeChild rỗng, thêm node mới chứa thông tin của text.
        Node *temp = createNode(text);
        dad->NodeChild.push_back(temp);
        return;
    }

    // Duyệt qua tất cả các nút con của nút cha
    for (auto &nodePtr : dad->NodeChild)
    {
        for (int i = nodePtr->info.length() - 1; i >= 0; i--)
        {
            // So sánh đoạn đầu của text và nodePtr
            if (text.find(nodePtr->info.substr(0, 1 + i)) == 0)
            {
                // Nếu phần đầu của Text và NodePtr giống nhau, thực hiện tách doạn đó ra
                if (i == (nodePtr->info.length() - 1))
                {
                    nodePtr->popular++;
                    if (text.substr(i + 1) == "")
                        return; // Điều kiện dừng nếu text đã được xử lý hết
                    return createSuffix(nodePtr, text.substr(i + 1));
                }
                else
                {
                    // Cắt phần giống nhau của nodePtr ra và đưa vào vector
                    Node *temp = createNode(nodePtr->info.substr(i + 1));
                    temp->NodeChild = nodePtr->NodeChild;
                    nodePtr->NodeChild.clear();
                    nodePtr->NodeChild.push_back(temp);
                    nodePtr->popular++;
                    nodePtr->info = nodePtr->info.substr(0, i + 1);
                    // Cắt phần giống nhau của text rồi đưa vào nodePtr
                    if (text.substr(i + 1) == "")
                        return; // Điều kiện dừng nếu text đã được xử lý hết
                    return createSuffix(nodePtr, text.substr(i + 1));
                }
            }
        }
    }

    // Thêm 1 nút mới vào danh sách nút con của nút cha nếu không tìm thấy phần giống nhau
    Node *temp = createNode(text);
    dad->NodeChild.push_back(temp);
}


void add(Node *&root, string text)
{
    int length = text.length() - 1;

    // Duyệt qua từng kí tự của từ
    for (int i = 0; i <= length; i++)
    {
        // Tạo một đoạn con của từ bắt đầu từ vị trí cuối cùng của từ và có độ dài i
        string temp = text.substr(length - i);

        // Thêm đoạn con này vào cây tiền tố, sử dụng hàm createSuffix
        createSuffix(root, temp);
    }
}


void forv(int n)
{
	for (int i = 0; i < n; i++)
		cout << "          ";
}

void xuat(Node *dad, int n, int &a)// hàm in chuỗi 
{
	for (Node *ptr : dad->NodeChild)// cho nodePtr chạy duyệt theo mức 
	{
		if (ptr != *dad->NodeChild.begin())
			forv(n);// tạo các khoảng trống dựa trên mức và điều kiện là biến tạm ptr khác với node bắt đầu
		else if (a != 0)
			cout << "      ";
		cout << ptr->info << "*--";
		a = a++;
		xuat(ptr, n + 1, a);// gọi đệ quy lần nữa để xét xuống mức tiếp theo
		cout << endl;
	}
}

void clearScreen()
{
#ifdef _WIN32
	system("cls");// hàm xóa làm sạch màn hình console
#else
	// Trường hợp hệ điều hành khác, có thểa sử dụng các phương pháp khác tùy thuộc vào hệ điều hành
	system("clear");
#endif
}
void suggestWords(Node* dad, string suggestion, string cat, vector<string>& temp)
{
    for (Node* child : dad->NodeChild)
    {
        if (suggestion == "")
        {
            // Nếu từ gợi ý rỗng, thêm từ hiện tại vào danh sách đề xuất
            temp.push_back(cat + child->info);
            // Tiếp tục đệ quy để xem có từ con nào khác không
            suggestWords(child, suggestion, cat + child->info, temp);
        }
        else
        {
            for (int i = child->info.length() - 1; i >= 0; i--)
            {
                if (child->info < suggestion)
                {
                    // Nếu từ con nhỏ hơn từ gợi ý
                    if (suggestion.find(child->info.substr(0, 1 + i)) == 0)
                    {
                        if (suggestion.substr(i + 1) == "")
                        {
                            // Nếu từ gợi ý hoàn tất, thêm từ hiện tại vào danh sách đề xuất
                            suggestion = "";
                            temp.push_back(cat + child->info);
                            // Tiếp tục đệ quy để xem có từ con nào khác không
                            return suggestWords(child, suggestion, cat + child->info, temp);
                        }
                        else
                        {
                            // Nếu từ gợi ý chưa hoàn tất, tiếp tục đệ quy với từ con và phần còn lại của từ gợi ý
                            suggestWords(child, suggestion.substr(i + 1), cat + child->info, temp);
                        }
                    }
                }
                else
                {
                    // Nếu từ con lớn hơn hoặc bằng từ gợi ý
                    if (child->info.find(suggestion) == 0)
                    {
                        // Nếu từ con chứa đủ từ gợi ý, thêm từ hiện tại vào danh sách đề xuất
                        suggestion = "";
                        temp.push_back(cat + child->info);
                        // Tiếp tục đệ quy để xem có từ con nào khác không
                        return suggestWords(child, suggestion, cat + child->info, temp);
                    }
                }
            }
        }
    }
}


void LongestCommonString(Node *dad, int thanhphan, string a, vector<string> &temp)
{
    for (Node *ptr : dad->NodeChild)
    {
        if (ptr->popular > 0)
        {
            // Xây dựng các chuỗi chung dài nhất từ các nút lá của cây tiền tố
            temp.push_back(a + ptr->info + to_string(thanhphan));
            LongestCommonString(ptr, thanhphan + 1, a + ptr->info, temp);
        }
    }
}

void LongestCommonString(Node *dad)
{
    string a = "";
    int thanhphan = 1, b;
    vector<string> temp;
    LongestCommonString(dad, thanhphan, a, temp);

    // Tìm chiều dài lớn nhất trong vector temp
    for (string ptr : temp)
    {
        b = stoi(ptr.substr(ptr.length() - 1));
        if (b > thanhphan)
            thanhphan = b;
    }

    // In ra chuỗi chung dài nhất
    cout << endl;
    cout << "Chuoi dai nhat: " << endl;
    for (string ptr : temp)
    {
        b = stoi(ptr.substr(ptr.length() - 1));
        if (b == thanhphan)
            cout << ptr.substr(0, ptr.length() - 1) << endl;
    }
}

void menu()
{
	string text, temp = "", cat = "";
	string suggestion;
	vector<string> tempp;
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
			int n, a;
			n = 0;
			a = 0;
			xuat(root, n, a);
			break;
		case 3:
			LongestCommonString(root);
			break;
		}

	} while (action != 0);
}
int main()
{
	menu();
	return 0;
}