#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <Windows.h>
using namespace std;

struct Person {         // структура для записи информации
	char full_name[50];    // фамилия
	int id;        // личный номер
};
Person buf;       // с которой мы будем работать
 
struct Tree {     // структура дерева
	Person info;     
	Tree* left;
	Tree* right;
};
Tree *main_root;     // корень

Tree* Create(Person user_inf) {          // создание дерева
	Tree *cur = new Tree;
	cur->info = user_inf;
	cur->left = cur->right = NULL;
	return cur;
}

void Add(Tree* root1, Person user_inf) {       // добавление нового элемента
	Tree* prev = NULL, *cur;
	bool find = true;
	cur = root1;
	while (cur && find) {            // поиск места
		prev = cur;
		if (user_inf.id == cur->info.id) {
			find = false;
			cout << "Человек с таким ID уже существует" << endl;
		}
		else 
			if (user_inf.id > cur->info.id) {
				cur = cur->right;
			}
			else cur = cur->left;
	}

	if (find) {      // запись элемента в дерево
		cur = Create(user_inf);
		if (user_inf.id < prev->info.id) prev->left = cur;
		else prev->right = cur;

	}
}

void View(Tree* root) {     // просмотр дерева
	if (root != NULL) { 
		View(root->right);  
		cout << root->info.id << "|" << root->info.full_name << endl;  
		View(root->left);
	} 
}

void Delete(Tree * root) {       // удаление 
	if (root != NULL) {
		Delete(root->left);
		Delete(root->right);
		delete root;
	}
}

void Search(Tree * root, int key) {
	Tree *cur = root;
	bool search_result = false;

	while (cur != NULL) {  // поиск
		if (cur->info.id == key) {
			search_result = true;   // если нашли - true
			break;
		}
		if (key < cur->info.id) {
			cur = cur->left;
		}
		else {
			cur = cur->right;
		}
	}

	if (search_result == false) {    // заканчиваем программу если нет результата поиска 
		cout << "Человек с таким ID не найден" << endl;
		Sleep(1000);
		system("cls");
	}
	else {
		cout << "Результат поиска:  " << cur->info.full_name << " || " << cur->info.id << endl;
		system("pause");
		system("cls");
	}
}

Tree* Delete_defenite(Tree *root, int key) {
	Tree* del = root, *prev = NULL;       // del - удаляемый, prev - предыдущий
	int a = 0;  // для определения к правому или левому поддереву предыдущего элемаента относится искомый
	Tree* replacement = NULL, *replacement_prev = NULL;
	bool search_result = false;
	while (del != NULL) {  // поиск
		if (del->info.id == key) {
			search_result = true;   // если нашли - true
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
	if (search_result == false) {    // заканчиваем программу если нет результата поиска 
		return root;
	}
	
	else {
		if (root->info.id == key && root->left == NULL && root->right == NULL) {   // если корень не имеет потомков
			delete root;
			return NULL;
		}
		if (root->info.id == key && root->right == NULL) {
			replacement = root->left;
			delete root;
			return replacement;
		}
		if (root->info.id == key) {    // если удаляемый элемент - корень
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
		if (del->left == NULL && del->right == NULL) {      // если у искомого нет потомков
			if (a == 1) {
				prev->left = NULL;
				return root;
			}
			if (a == 2) {
				prev->right = NULL;
				return root;
			}
		}

		if (del->left != NULL && del->right == NULL) {     // если у искомого один потомок - левый
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

		if (del->left == NULL && del->right != NULL) {    // если у искомого один потомок - правый
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

		else {                 // в другом случае
			if (a == 1) {
				replacement_prev = del;    // предыдущий заменяемому равен удаляемому
				replacement = del->right;  // заменяемый равен правому от удаляемого
				if (replacement->left == NULL) {   // если его левое значение навно нулю, то просто заменяем удаляемый элемент этим, сохраняя его правую часть, и привязывая к его левой части левую часть от удаляемого
					prev->left = replacement;
					replacement->left = del->left;
					delete del;
					return root;
				}
				while (replacement->left != NULL) {   // в другом случае ищем минимальный элемент (самый левый) в правом поддереве
					replacement_prev = replacement;
					replacement = replacement->left;
				}
				prev->left = replacement;      // всё привязываем
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

void Save(Tree *root, FILE *file ) {                 // сохранение всей информации в файл
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
	int password[4] = { 49, 49, 50, 50 }, user_password[4];         // для работы с удалением элементов базы
	int key; // для поиска и удаления
	int b;
	int g = 0;

	
	FILE *file_read = fopen("data_base.txt", "rb");   // открытие файла для чтения информации 
	if (file_read == NULL) {
		printf("Ошибка при создании файла \n");
		return 2;
	}
	Person bufer = { 0, 0 };
	long i;
	fseek(file_read, 0, SEEK_END);
	i = ftell(file_read);
	if (i > 0) {
		rewind(file_read); // возвращение курсора в начало файла
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




	FILE *file_save = fopen("data_base.txt", "wb");   // открытие файла для сохранения информации 
	if (file_save == NULL) {
		printf("Ошибка при создании файла \n");
		return 2;
	}

	while (true) {
		cout << " \n \n Действия с базой: \n 1. Добавление \n 2. Поиск \n 3. Просмотр \n 4. Удаление элементов базы \n 5. Выход \n ------------------------------ \n \n";
		choise = _getch();
		switch (choise)
		{
		case 49: 
			cout << "Введите ФИО и личный номер человека" << endl;
			if (main_root == NULL) {          // если дерева нет - создаём
				cin >> buf.full_name;
				cin >> buf.id;
				main_root = Create(buf);
			}
			else {                    // иначе - добавляем
				cin >> buf.full_name;
				cin >> buf.id;
				Add(main_root, buf);
			}
			system("cls");
			break;
		case 50: 
			cout << "Введите личный номер для поиска:  ";
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
				cout << "База пуста" << endl;
				Sleep(500);
				system("cls");
				break;
			}
		case 52: 
			if (main_root != NULL) {
				cout << "Введите пароль администратора: ";  
				for (int i = 0; i < 4; i++) {
					user_password[i] = _getch();
					cout << "*";
				}
				cout << endl;
				g = 0; // переменная для проверки правильности ввода пароля
				for (int i = 0; i < 4; i++) {
					if (password[i] == user_password[i]) {
						g++;
					}
				}
				if (g == 4) {
					cout << " 1. Удаление определённых субъектов \n 2. Очистка всей базы" << endl;
					cin >> b;
					if (b == 1) {
						cout << "Введите личный номер для удаления:  ";
						cin >> key;
						main_root = Delete_defenite(main_root, key);
						cout << "Выполнено успешно" << endl;
						Sleep(500);
						system("cls");
						break;
					}
					if (b == 2) {
						Delete(main_root);
						main_root = NULL;
						cout << "Выполнено успешно" << endl;
						Sleep(500);
						system("cls");
						break;
					}
					else {
						break;
					}
				}
				else {
					cout << "Неверный пароль" << endl;
					Sleep(500);
					system("cls");
					break;
				}
			}
			else {
				cout << "База пуста" << endl;
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