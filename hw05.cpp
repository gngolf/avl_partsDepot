// Grant Norton
// HW05
// due 7/19/2019

#include<iostream>
#include<string>
#include<iomanip>
#include<sstream>
using namespace std;

class AVLTree {
private:
	struct Node {
		int partnum;
		string description;
		int qty;
		double price;
		Node* left;
		Node* right;
	};
	Node* root;

	void clear(Node*& r);
	int height(Node* r);
	void rotateRight(Node*& r);
	void rotateLeft(Node*& r);
	void rotateRightLeft(Node*& r);
	void rotateLeftRight(Node*& r);
	int difference(Node* r);
	void balance(Node*& r);
	void append(Node*& r, int num, int qty, string desc, double prc);
	void remove(Node*& r, int num);
	string getOutOfStock(Node* r);
	string getPart(Node* r, int num);

public:
	AVLTree();
	~AVLTree();
	void clear();
	void append(int num, int qty, string desc, double prc);
	void remove(int num);
	string getOutOfStock();	
	string getPart(int num);

};

AVLTree::AVLTree() {
	root = nullptr;
}

AVLTree::~AVLTree() {
	clear();
}

void AVLTree::clear(Node*& r) {
	if (r == nullptr)
		return;
	clear(r->left);
	clear(r->right);
	delete r;
}

void AVLTree::clear() {
	clear(root);
	root = nullptr;
}

int AVLTree::height(Node* r) {
	if (r == nullptr)
		return (0);
	else {
		int L, R;
		L = height(r->left);
		R = height(r->right);
		
		if (L > R)
			return (L + 1);
		else
			return (R + 1);
	}
}

void AVLTree::rotateRight(Node*& r) {
	Node* p = r->left;
	r->left = p->right;
	p->right = r;
	r = p;
}

void AVLTree::rotateLeft(Node*& r) {
	Node* p = r->right;
	r->right = p->left;
	p->left = r;
	r = p;
}

void AVLTree::rotateRightLeft(Node*& r) {
	rotateRight(r->right);
	rotateLeft(r);
}

void AVLTree::rotateLeftRight(Node*& r) {
	rotateLeft(r->left);
	rotateRight(r);
}

int AVLTree::difference(Node* r) {
	int bf;
	bf = (height(r->right) - height(r->left));
	return bf;
}

void AVLTree::balance(Node*& r) {
	if (difference(r) == -2) {
		if (difference(r->left) <= -1)
			rotateRight(r);
		else
			rotateLeftRight(r);
	}
	else if (difference(r) == 2) {
		if (difference(r->right) >= 1)
			rotateLeft(r);
		else
			rotateRightLeft(r);
	}
}

void AVLTree::append(Node*& r, int num, int qty, string desc, double prc) {
	if (r == nullptr) {
		r = new Node;
		r->partnum = num;
		r->qty = qty;
		r->description = desc;
		r->price = prc;
		r->left = nullptr;
		r->right = nullptr;
	}
	else if (num < r->partnum) {
		append(r->left, num, qty, desc, prc);
		balance(r);
	}
	else {
		append(r->right, num, qty, desc, prc);
		balance(r);
	}
}

void AVLTree::append(int num, int qty, string desc, double prc) {
	append(root, num, qty, desc, prc);
}

void AVLTree::remove(Node*& r, int num) { 
	if (r == nullptr)
		return;
	else if (r->partnum == num) {
		if (r->left == nullptr && r->right == nullptr) {
			delete r;
			r = nullptr;
		}
		else if (r->left != nullptr && r->right == nullptr) {
			Node* temp = r;
			r = r->left;
			delete temp;
			balance(r);
		}
		else if (r->left == nullptr && r->right != nullptr) {
			Node* temp = r;
			r = r->right;
			delete temp;
			balance(r);
		}
		else {
			Node* temp = r->right;
			while (temp->left != nullptr) {
				temp = temp->left;
			}
			temp->left = r->left;
			temp = r;
			r = r->right;
			delete temp;
			balance(r);
		}
	}
	else {
		remove(r->left, num);
		remove(r->right, num);
	}
	
}

void AVLTree::remove(int num) {
	remove(root, num);
}

string AVLTree::getOutOfStock(Node* r) {  
	if (r == nullptr)
		return("");
	string str;
	str += getOutOfStock(r->left);
	str += getOutOfStock(r->right);
	if (r->qty == 0) {
		stringstream ss;
		ss << fixed;
		ss << setprecision(2);
		ss << r->price;
		string prc = ss.str();
		str += "  " + to_string(r->partnum) + ", " + r->description + ", " + prc + ", " + to_string(r->qty) + "\n";
		return(str);
	}
	else
		return(str);
}

string AVLTree::getOutOfStock() {
	string s;
	s = getOutOfStock(root);
	return s;
}

string AVLTree::getPart(Node* r, int num) {
	if (r == nullptr)
		return("  OUT OF STOCK \n");
	else if (r->partnum == num) {
		stringstream ss;
		ss << fixed;
		ss << setprecision(2);
		ss << r->price;
		string prc = ss.str();
		return("  " + to_string(r->partnum) + "\n  " + r->description + "\n  " + prc + "\n  " + to_string(r->qty) + "\n");
	}
	else if (num < r->partnum)
		return getPart(r->left, num);
	else 
		return getPart(r->right, num);
}

string AVLTree::getPart(int num) {
	string s;
	s = getPart(root, num);
	return s;
}

int main() {

	AVLTree parts;
	char choice;

	do
	{
		cout << "MENU" << endl;
		cout << "  (A)dd a part" << endl;
		cout << "  (R)emove a part" << endl;
		cout << "  (S)earch for a part" << endl;
		cout << "  (O)ut of stock" << endl;
		cout << "  (Q)uit" << endl;

		cout << "[Enter a choice]: ";
		cin >> choice;
		choice = tolower(choice);

		if (choice == 'a') {
			int prtNum;
			double prc;
			string des;
			int qty;

			cout << "  Enter part description: ";
			cin >> des;
			cout << "  Enter the price: ";
			cin >> prc;
			cout << "  Enter the part number: ";
			cin >> prtNum;
			cout << "  Enter the quantity on hand: ";
			cin >> qty;

			parts.append(prtNum, qty, des, prc);
		}

		else if (choice == 'r') {
			int prtNum;

			cout << "  Enter the part number to be removed: ";
			cin >> prtNum;
			parts.remove(prtNum);
		}

		else if (choice == 's') {
			int prtNum;

			cout << "  Enter the part number to search for: ";
			cin >> prtNum;
			cout << endl;
			cout << parts.getPart(prtNum) << endl;
		}

		else if (choice == 'o') {
			string str = parts.getOutOfStock();
			if (str.length() > 0){
				cout << "  The following items are out of stock:\n" << endl;
				cout << str;
				cout << endl;
			}
			else 
				cout << "  \nNO ITEMS OUT OF STOCK \n\n";
		}
	} while (choice != 'q');

	return 0;
};