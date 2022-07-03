#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <Windows.h>
using namespace std;

struct Person {         // ��������� ��� ������ ����������
	char full_name[50];    // �������
	int id;        // ������ �����
};
Person buf;       // � ������� �� ����� ��������
 
struct Tree {     // ��������� ������
	Person info;     
	Tree* left;
	Tree* right;
};
Tree *main_root;     // ������

Tree* Create(Person user_inf) {          // �������� ������
	Tree *cur = new Tree;
	cur->info = user_inf;
	cur->left = cur->right = NULL;
	return cur;
}

void Add(Tree* root1, Person user_inf) {       // ���������� ������ ��������
	Tree* prev = NULL, *cur;
	bool find = true;
	cur = root1;
	while (cur && find) {            // ����� �����
		prev = cur;
		if (user_inf.id == cur->info.id) {
			find = false;
			cout << "������� � ����� ID ��� ����������" << endl;
		}
		else 
			if (user_inf.id > cur->info.id) {
				cur = cur->right;
			}
			else cur = cur->left;
	}

	if (find) {      // ������ �������� � ������
		cur = Create(user_inf);
		if (user_inf.id < prev->info.id) prev->left = cur;
		else prev->right = cur;

	}
}

void View(Tree* root) {     // �������� ������
	if (root != NULL) { 
		View(root->right);  
		cout << root->info.id << "|" << root->info.full_name << endl;  
		View(root->left);
	} 
}

void Delete(Tree * root) {       // �������� 
	if (root != NULL) {
		Delete(root->left);
		Delete(root->right);
		delete root;
	}
}

void Search(Tree * root, int key) {
	Tree *cur = root;
	bool search_result = false;

	while (cur != NULL) {  // �����
		if (cur->info.id == key) {
			search_result = true;   // ���� ����� - true
			break;
		}
		if (key < cur->info.id) {
			cur = cur->left;
		}
		else {
			cur = cur->right;
		}
	}

	if (search_result == false) {    // ����������� ��������� ���� ��� ���������� ������ 
		cout << "������� � ����� ID �� ������" << endl;
		Sleep(1000);
		system("cls");
	}
	else {
		cout << "��������� ������:  " << cur->info.full_name << " || " << cur->info.id << endl;
		system("pause");
		system("cls");
	}
}

Tree* Delete_defenite(Tree *root, int key) {
	Tree* del = root, *prev = NULL;       // del - ���������, prev - ����������
	int a = 0;  // ��� ����������� � ������� ��� ������ ��������� ����������� ��������� ��������� �������
	Tree* replacement = NULL, *replacement_prev = NULL;
	bool search_result = false;
	while (del != NULL) {  // �����
		if (del->info.id == key) {
			search_result = true;   // ���� ����� - true
			break;
		}
		prev = del;
		if (key < del->info.id) {
			del = del->left;
			a = 1;
		}
		else {
			del = del->right;
			a = 2;
		}
	}
	if (search_result == false) {    // ����������� ��������� ���� ��� ���������� ������ 
		return root;
	}
	
	else {
		if (root->info.id == key && root->left == NULL && root->right == NULL) {   // ���� ������ �� ����� ��������
			delete root;
			return NULL;
		}
		if (root->info.id == key && root->right == NULL) {
			replacement = root->left;
			delete root;
			return replacement;
		}
		if (root->info.id == key) {    // ���� ��������� ������� - ������
			replacement = del->right;

			if (replacement->left == NULL) {
				replacement->left = del->left;
				delete del;
				return replacement;
			}

			while (replacement->left != NULL) {
				replacement_prev = replacement;
				replacement = replacement->left;
			}
			replacement->left = del->left;
			replacement->right = del->right;
			delete del;
			return replacement;
		}
		if (del->left == NULL && del->right == NULL) {      // ���� � �������� ��� ��������
			if (a == 1) {
				prev->left = NULL;
				return root;
			}
			if (a == 2) {
				prev->right = NULL;
				return root;
			}
		}

		if (del->left != NULL && del->right == NULL) {     // ���� � �������� ���� ������� - �����
			if (a == 1) {
				prev->left = del->left;
				delete del;
				return root;
			}
			if (a == 2) {
				prev->right = del->left;
				delete del;
				return root;
			}
		}

		if (del->left == NULL && del->right != NULL) {    // ���� � �������� ���� ������� - ������
			if (a == 1) {
				prev->left = del->right;
				delete del;
				return root;
			}
			if (a == 2) {
				prev->right = del->right;
				delete del;
				return root;
			}
		}

		else {                 // � ������ ������
			if (a == 1) {
				replacement_prev = del;    // ���������� ����������� ����� ����������
				replacement = del->right;  // ���������� ����� ������� �� ����������
				if (replacement->left == NULL) {   // ���� ��� ����� �������� ����� ����, �� ������ �������� ��������� ������� ����, �������� ��� ������ �����, � ���������� � ��� ����� ����� ����� ����� �� ����������
					prev->left = replacement;
					replacement->left = del->left;
					delete del;
					return root;
				}
				while (replacement->left != NULL) {   // � ������ ������ ���� ����������� ������� (����� �����) � ������ ���������
					replacement_prev = replacement;
					replacement = replacement->left;
				}
				prev->left = replacement;      // �� �����������
				replacement->left = del->left;
				replacement->right = del->right;
				replacement_prev->left = NULL;
				delete del;
				return root;
			}
			if (a == 2) {
				replacement_prev = del;
				replacement = del->right;

				if (replacement->left == NULL) {
					prev->right = replacement;
					replacement->left = del->left;
					delete del;
					return root;
				}

				while (replacement->left != NULL) {
					replacement_prev = replacement;
					replacement = replacement->left;
				}
		
				prev->right = replacement;
				replacement->left = del->left;
				replacement->right = del->right;
				
				delete del;
				return root;
			}
		}
	}
}

void Save(Tree *root, FILE *file ) {                 // ���������� ���� ���������� � ����
	if (root != NULL) {
		Save(root->left, file);
		fwrite(&root->info, 1, sizeof(Person), file);
		Save(root->right, file);
	}
	return;
}


int main() {
	setlocale(LC_ALL, "Russian");
	int choise;
	int password[4] = { 49, 49, 50, 50 }, user_password[4];         // ��� ������ � ��������� ��������� ����
	int key; // ��� ������ � ��������
	int b;
	int g = 0;

	
	FILE *file_read = fopen("data_base.txt", "rb");   // �������� ����� ��� ������ ���������� 
	if (file_read == NULL) {
		printf("������ ��� �������� ����� \n");
		return 2;
	}
	Person bufer = { 0, 0 };
	long i;
	fseek(file_read, 0, SEEK_END);
	i = ftell(file_read);
	if (i > 0) {
		rewind(file_read); // ����������� ������� � ������ �����
		do {
			fread(&bufer, 1, sizeof(Person), file_read);
			if (main_root == NULL) {
				main_root = Create(bufer);
			}
			else Add(main_root, bufer);
		} while (!feof(file_read));
	}
	system("cls"); 
	fclose(file_read);




	FILE *file_save = fopen("data_base.txt", "wb");   // �������� ����� ��� ���������� ���������� 
	if (file_save == NULL) {
		printf("������ ��� �������� ����� \n");
		return 2;
	}

	while (true) {
		cout << " \n \n �������� � �����: \n 1. ���������� \n 2. ����� \n 3. �������� \n 4. �������� ��������� ���� \n 5. ����� \n ------------------------------ \n \n";
		choise = _getch();
		switch (choise)
		{
		case 49: 
			cout << "������� ��� � ������ ����� ��������" << endl;
			if (main_root == NULL) {          // ���� ������ ��� - ������
				cin >> buf.full_name;
				cin >> buf.id;
				main_root = Create(buf);
			}
			else {                    // ����� - ���������
				cin >> buf.full_name;
				cin >> buf.id;
				Add(main_root, buf);
			}
			system("cls");
			break;
		case 50: 
			cout << "������� ������ ����� ��� ������:  ";
			cin >> key;
			Search(main_root, key);
			break;
		case 51:        
			if (main_root != NULL) {        
				View(main_root);
				system("pause");
				system("cls");
				break;
			}
			else {
				cout << "���� �����" << endl;
				Sleep(500);
				system("cls");
				break;
			}
		case 52: 
			if (main_root != NULL) {
				cout << "������� ������ ��������������: ";  
				for (int i = 0; i < 4; i++) {
					user_password[i] = _getch();
					cout << "*";
				}
				cout << endl;
				g = 0; // ���������� ��� �������� ������������ ����� ������
				for (int i = 0; i < 4; i++) {
					if (password[i] == user_password[i]) {
						g++;
					}
				}
				if (g == 4) {
					cout << " 1. �������� ����������� ��������� \n 2. ������� ���� ����" << endl;
					cin >> b;
					if (b == 1) {
						cout << "������� ������ ����� ��� ��������:  ";
						cin >> key;
						main_root = Delete_defenite(main_root, key);
						cout << "��������� �������" << endl;
						Sleep(500);
						system("cls");
						break;
					}
					if (b == 2) {
						Delete(main_root);
						main_root = NULL;
						cout << "��������� �������" << endl;
						Sleep(500);
						system("cls");
						break;
					}
					else {
						break;
					}
				}
				else {
					cout << "�������� ������" << endl;
					Sleep(500);
					system("cls");
					break;
				}
			}
			else {
				cout << "���� �����" << endl;
				_getch();
				system("cls");
				break;
			}
		case 53:
			if (main_root != NULL) {
				Save(main_root, file_save);
				fclose(file_save);
			}

			Delete(main_root);
			return 0;
		default:
			break;
		}
	}
}