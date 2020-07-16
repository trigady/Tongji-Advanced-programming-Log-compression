#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

using namespace std;

//首先定义huffman压缩方法
struct huffnode {
	bool flag = true;
	char c = '0';
	int value = 0;
	huffnode *left = NULL;
	huffnode *right = NULL;
}*head_for_huffman, *head;
vector<huffnode> vec_for_huffman;
char trans_for_huffman[256][256];
char transnum_for_huffman = 0;
int wordnum_for_huffman = 0;
vector<bool> trans_temp_for_huffman;
char buf_for_huffman = 0;
short bufnum_for_huffman = 0;

int findmin() {
	int min, mini;
	for (unsigned int i = 0; i < vec_for_huffman.size(); i++) {
		if (i == 0) {
			min = vec_for_huffman[i].value;
			mini = 0;
		}
		else {
			if (min > vec_for_huffman[i].value) {
				min = vec_for_huffman[i].value;
				mini = i;
			}
		}
	}
	return mini;
}

void transcode(huffnode *p) {
	if (!p->flag) {
		for (unsigned int i = 0; i < trans_temp_for_huffman.size(); i++) {
			if (trans_temp_for_huffman[i]) {
				trans_for_huffman[(int)(p->c)][i] = '1';
			}
			else {
				trans_for_huffman[(int)(p->c)][i] = '0';
			}
		}
		transnum_for_huffman++;
		trans_for_huffman[(int)(p->c)][trans_temp_for_huffman.size()] = '\0';
		return;
	}
	else {
		trans_temp_for_huffman.push_back(false);
		transcode(p->left);
		trans_temp_for_huffman.pop_back();
		trans_temp_for_huffman.push_back(true);
		transcode(p->right);
		trans_temp_for_huffman.pop_back();
	}
}

void huffmaketree(const char add[]) {
	int alpha[256], o1;
	char temp;
	fstream fp;
	fp.open(add, ios::in);
	huffnode *p = NULL, *uu = NULL;

	if (!fp.is_open()) {
		cerr << "这个文件打不开！！！";
		exit(-1);
	}
	for (int i = 0; i < 256; i++) {
		alpha[i] = 0;
	}
	for (;;) {
		fp >> noskipws >> temp;
		wordnum_for_huffman++;
		alpha[(int)temp]++;
		if (fp.eof()) {
			break;
		}
	}
	fp.close();

	for (int i = 0; i < 256; i++) {
		if (alpha[i] != 0) {
			p = new huffnode;
			p->c = (char)i;
			p->flag = false;
			p->value = alpha[i];
			vec_for_huffman.push_back(*p);
		}
	}
	for (;;) {
		if (vec_for_huffman.size() == 1) {
			head_for_huffman = &vec_for_huffman[0];
			vec_for_huffman.clear();
			break;
		}
		uu = new huffnode;

		p = new huffnode;
		o1 = findmin();
		p->c = vec_for_huffman[o1].c;
		p->flag = vec_for_huffman[o1].flag;
		p->value = vec_for_huffman[o1].value;
		p->left = vec_for_huffman[o1].left;
		p->right = vec_for_huffman[o1].right;
		vec_for_huffman.erase(vec_for_huffman.begin() + o1);
		uu->left = p;

		p = new huffnode;
		o1 = findmin();
		p->c = vec_for_huffman[o1].c;
		p->flag = vec_for_huffman[o1].flag;
		p->value = vec_for_huffman[o1].value;
		p->left = vec_for_huffman[o1].left;
		p->right = vec_for_huffman[o1].right;
		vec_for_huffman.erase(vec_for_huffman.begin() + o1);
		uu->right = p;

		uu->flag = true;
		uu->value = uu->left->value + uu->right->value;
		vec_for_huffman.push_back(*uu);
	}
	transcode(head_for_huffman);
}

void huffouttree(const char addin[], const char addout[]) {
	fstream inn, outt;
	char temp, tt, lil;
	inn.open(addin, ios::in | ios::binary);
	int tool;

	if (!inn.is_open()) {
		cerr << "文件打不开！！！！！！";
		exit(-1);
	}
	outt.open(addout, ios::out | ios::binary);
	if (!outt.is_open()) {
		cerr << "文件打不开！！！！！！";
		exit(-1);
	}
	outt.write((char*)&transnum_for_huffman, sizeof(transnum_for_huffman));
	outt.write((char*)&wordnum_for_huffman, sizeof(wordnum_for_huffman));
	for (int i = 0; i < 256; i++) {
		if (trans_for_huffman[i][0] != '1'&&trans_for_huffman[i][0] != '0') {
			continue;
		}
		temp = i;
		for (int k = 0; k < 8; k++) {
			tt = (temp >> (7 - k)) % 2;
			buf_for_huffman = buf_for_huffman * (char)2 + tt;
			bufnum_for_huffman++;
			if (bufnum_for_huffman == 8) {
				outt.write((char*)&buf_for_huffman, sizeof

				(buf_for_huffman));
				bufnum_for_huffman = 0;
				buf_for_huffman = 0;
			}
		}
		for (lil = 0; trans_for_huffman[i][lil] != '\0'; lil++);
		for (int k = 0; k < 8; k++) {
			tt = (lil >> (7 - k)) % 2;
			buf_for_huffman = buf_for_huffman * (char)2 + tt;
			bufnum_for_huffman++;
			if (bufnum_for_huffman == 8) {
				outt.write((char*)&buf_for_huffman, sizeof

				(buf_for_huffman));
				bufnum_for_huffman = 0;
				buf_for_huffman = 0;
			}
		}
		tool = 0;
		for (int j = 0; trans_for_huffman[i][j] != '\0'; j++) {
			tool = tool * 2 + trans_for_huffman[i][j] - '0';
		}
		outt.write((char*)&tool, sizeof(tool));
	}

	for (;;) {
		inn >> noskipws >> temp;
		for (int i = 0; trans_for_huffman[(int)temp][i] != '\0'; i++) {
			buf_for_huffman = buf_for_huffman * (char)2 + (trans_for_huffman

				[(int)temp][i] - '0');
			bufnum_for_huffman++;
			if (bufnum_for_huffman == 8) {
				outt.write((char*)&buf_for_huffman, sizeof

				(buf_for_huffman));
				bufnum_for_huffman = 0;
				buf_for_huffman = 0;
			}
		}
		if (inn.eof()) {
			for (;;) {
				if (bufnum_for_huffman == 0) {
					break;
				}
				buf_for_huffman = buf_for_huffman * (char)2;
				bufnum_for_huffman++;
				if (bufnum_for_huffman == 8) {
					outt.write((char*)&buf_for_huffman, sizeof

					(buf_for_huffman));
					bufnum_for_huffman = 0;
					buf_for_huffman = 0;
				}
			}
			break;
		}
	}
	inn.close();
	outt.close();
}

void huffman(const char add1[], const char add2[]) {
	huffmaketree(add1);
	huffouttree(add1, add2);
}

//这里给出huffman的解码方式
char buf_for_dehuffman;
short bufnum_for_dehuffman = 8;
fstream fin_for_dehuffman, fout_for_dehuffman;

char dehuffman_get() {
	if (bufnum_for_dehuffman == 8) {
		fin_for_dehuffman.read((char*)&buf_for_dehuffman, sizeof(buf_for_dehuffman));
		bufnum_for_dehuffman = 0;
	}
	bufnum_for_dehuffman++;
	return (buf_for_dehuffman >> (8 - bufnum_for_dehuffman)) % 2;
}

void dehuffman(const char inn[], const char outt[]) {
	char transnum;
	int wordnum;
	char wordc, too, haha;
	int len, tool;
	huffnode *head = NULL, *p = NULL, *q = NULL;

	fin_for_dehuffman.open(inn, ios::in | ios::binary);
	if (!fin_for_dehuffman.is_open()) {
		cerr << "文件打不开！！！！！！";
		exit(-1);
	}
	fout_for_dehuffman.open(outt, ios::out | ios::binary);
	if (!fout_for_dehuffman.is_open()) {
		cerr << "文件打不开！！！！！！";
		exit(-1);
	}

	fin_for_dehuffman.read((char*)&transnum, sizeof(transnum));
	fin_for_dehuffman.read((char*)&wordnum, sizeof(wordnum));

	for (int i = 0; i < transnum; i++) {
		wordc = 0;
		for (int uhi = 0; uhi < 8; uhi++) {
			if (bufnum_for_dehuffman == 8) {
				fin_for_dehuffman.read((char*)&buf_for_dehuffman, sizeof

				(buf_for_dehuffman));
				bufnum_for_dehuffman = 0;
			}
			bufnum_for_dehuffman++;
			haha = (buf_for_dehuffman >> (8 - bufnum_for_dehuffman)) % 2;
			wordc = wordc * 2 + haha;
		}
		len = 0;
		for (int uhi = 0; uhi < 8; uhi++) {
			if (bufnum_for_dehuffman == 8) {
				fin_for_dehuffman.read((char*)&buf_for_dehuffman, sizeof

				(buf_for_dehuffman));
				bufnum_for_dehuffman = 0;
			}
			bufnum_for_dehuffman++;
			haha = ((buf_for_dehuffman >> (8 - bufnum_for_dehuffman)) % 2);
			len = len * 2 + haha;
		}
		if (i == 0) {
			head = new huffnode;
		}
		tool = 0;
		p = head;
		fin_for_dehuffman.read((char*)&tool, sizeof(tool));
		for (int j = 0; j < len; j++) {
			too = (tool >> (len - j - 1)) % 2;
			if (too == 0) {
				if (p->left == NULL) {
					q = new huffnode;
					p->left = q;
				}
				p = p->left;
			}
			else {
				if (p->right == NULL) {
					q = new huffnode;
					p->right = q;
				}
				p = p->right;
			}
			if (j == len - 1) {
				p->flag = false;
				p->c = wordc;
			}
		}
	}

	for (int i = 0; i < wordnum; i++) {
		p = head;
		for (;;) {
			if (p->flag == false) {
				break;
			}
			if (bufnum_for_dehuffman == 8) {
				fin_for_dehuffman.read((char*)&buf_for_dehuffman, sizeof

				(buf_for_dehuffman));
				bufnum_for_dehuffman = 0;
			}
			bufnum_for_dehuffman++;
			haha = (buf_for_dehuffman >> (8 - bufnum_for_dehuffman)) % 2;
			too = haha;
			if (too == 0) {
				p = p->left;
			}
			else {
				p = p->right;
			}
		}
		fout_for_dehuffman.write((char*)&(p->c), sizeof(p->c));
	}
	fin_for_dehuffman.close();
	fout_for_dehuffman.close();
}

//这里给出lz77的编码方式
const int WINDOW_for_lz77 = 8192;
vector <char> window_for_lz77;
const int ZOOM_for_lz77 = 100;
vector <char> zoom_for_lz77;

int getlen(int wi, int zo) {
	int ans = 0, window_for_lz77i = wi, zoom_for_lz77i = zo;
	for (;;) {
		if (window_for_lz77[window_for_lz77i] == zoom_for_lz77[zoom_for_lz77i]) {
			window_for_lz77i++;
			zoom_for_lz77i++;
			ans++;
			if (window_for_lz77i == window_for_lz77.size() || zoom_for_lz77i == zoom_for_lz77.size()) {
				return ans;
			}
		}
		else {
			return ans;
		}

	}
}

void lz77(const char inn[], const char outt[]) {
	fstream fin, fout;
	char temp;
	int zoom_for_lz77i, window_for_lz77i;
	bool flag = true;

	int len = 0, lenmax, leni;
	char buf;
	short buf2;

	fin.open(inn, ios::in | ios::binary);
	if (!fin.is_open()) {
		cerr << "文件打不开！！！！！";
		exit(-1);
	}
	fout.open(outt, ios::out | ios::binary);
	if (!fout.is_open()) {
		cerr << "文件打不开！！！！！";
		exit(-1);
	}

	for (int i = 0; i < ZOOM_for_lz77; i++) {
		fin >> noskipws >> temp;
		zoom_for_lz77.push_back(temp);
	}
	for (int i = 0; i < WINDOW_for_lz77; i++) {
		window_for_lz77.push_back('\0');
	}

	for (;;) {
		if (zoom_for_lz77.empty()) {
			break;
		}
		lenmax = -1;
		zoom_for_lz77i = window_for_lz77i = 0;
		for (int i = 0; i < WINDOW_for_lz77; i++) {
			if (window_for_lz77[i] == zoom_for_lz77[0]) {
				len = getlen(i, 0);
				if (len > lenmax) {
					lenmax = len;
					leni = i;
				}
			}
		}
		if (lenmax <= 3) {
			buf = '0';
			fout.write((char*)&buf, sizeof(buf));
			buf = zoom_for_lz77[0];
			fout.write((char*)&buf, sizeof(buf));
			window_for_lz77.erase(window_for_lz77.begin());
			window_for_lz77.push_back(zoom_for_lz77[0]);
			zoom_for_lz77.erase(zoom_for_lz77.begin());
			if (zoom_for_lz77.empty()) {
				break;
			}
			if (flag) {
				fin >> noskipws >> temp;
				zoom_for_lz77.push_back(temp);
			}
			if (fin.eof()) {
				flag = false;
			}
		}
		else {
			buf = '1';
			fout.write((char*)&buf, sizeof(buf));
			buf2 = leni;
			fout.write((char*)&buf2, sizeof(buf2));
			buf2 = lenmax;
			fout.write((char*)&buf2, sizeof(buf2));
			for (int uhi = 0; uhi < lenmax; uhi++) {
				window_for_lz77.erase(window_for_lz77.begin());
				window_for_lz77.push_back(zoom_for_lz77[0]);
				zoom_for_lz77.erase(zoom_for_lz77.begin());
				if (zoom_for_lz77.empty()) {
					break;
				}
				if (flag) {
					fin >> noskipws >> temp;
					zoom_for_lz77.push_back(temp);
				}
				if (fin.eof()) {
					flag = false;
				}
			}
			buf = zoom_for_lz77[0];
			fout.write((char*)&buf, sizeof(buf));
			window_for_lz77.erase(window_for_lz77.begin());
			window_for_lz77.push_back(zoom_for_lz77[0]);
			zoom_for_lz77.erase(zoom_for_lz77.begin());
			if (zoom_for_lz77.empty()) {
				break;
			}
			if (flag) {
				fin >> noskipws >> temp;
				zoom_for_lz77.push_back(temp);
			}
			if (fin.eof()) {
				flag = false;
			}
		}
	}
	fin.close();
	fout.close();
}

//这里给出lz77的解码方式
const int WINDOW_for_delz77 = 8192;
vector <char> window_for_delz77;

void delz77(const char inn[], const char outt[]) {
	fstream fin, fout;
	fin.open(inn, ios::in | ios::binary);
	char temp, buf;
	short buf2, tool;

	if (!fin.is_open()) {
		cerr << "文件打不开！！！！！";
		exit(-1);
	}
	fout.open(outt, ios::out | ios::binary);
	if (!fout.is_open()) {
		cerr << "文件打不开！！！！！";
		exit(-1);
	}

	for (int i = 0; i < WINDOW_for_delz77; i++) {
		window_for_delz77.push_back('\0');
	}

	for (;;) {
		if (fin.eof()) {
			break;
		}
		fin.read((char*)&buf, sizeof(buf));
		if (fin.eof()) {
			break;
		}
		if (buf == '0') {
			fin.read((char*)&buf, sizeof(buf));
			fout.write((char*)&buf, sizeof(buf));
			window_for_delz77.erase(window_for_delz77.begin());
			window_for_delz77.push_back(buf);
		}
		else {
			fin.read((char*)&buf2, sizeof(buf2));
			fin.read((char*)&tool, sizeof(tool));
			for (int i = 0; i < tool; i++) {
				if (buf2 >= window_for_delz77.size()) {
					continue;
				}
				temp = window_for_delz77[buf2];
				if (temp == '\0') {
					continue;
				}
				fout.write((char*)&temp, sizeof(temp));
				window_for_delz77.erase(window_for_delz77.begin());
				window_for_delz77.push_back(temp);
			}
			fin.read((char*)&buf, sizeof(buf));
			fout.write((char*)&buf, sizeof(buf));
			window_for_delz77.erase(window_for_delz77.begin());
			window_for_delz77.push_back(buf);
		}
	}
	fin.close();
	fout.close();
}

int main() {
	fstream fp;
	clock_t ta, tb, tb1;
	int size0, size1, size2;
	double t1, t2, t3, t4;

	fp.open("ser.log", ios::app);
	if (fp.is_open()) {
		fp.seekg(0, ios::end);
		size0 = fp.tellg();
		fp.close();
	}
	else {
		cerr << "这文件打不开！！！！";
		exit(-1);
	}
	cout << "原ser.log大小为：" << size0 << "字节。" << endl << endl;

	//cout << "************************************************************************" << endl;
	//cout << "                         以下演示huffman压缩                            " << endl;

	//ta = clock();
	//huffman("ser.log", "ser.huff");
	//cout << "huffman压缩成功！" << endl;
	//tb = clock();
	//cout << "huffman压缩文件用时：" << (t1 = ((double)tb - (double)ta)) / CLOCKS_PER_SEC << "秒" << endl;
	//ta = clock();
	//dehuffman("ser.huff", "ser.v1");
	//tb = clock();
	//cout << "huffman解压文件用时：" << (t3 = ((double)tb - (double)ta)) / CLOCKS_PER_SEC << "秒" << endl;
	//cout << "------------------------------------------------" << endl;
	//fp.open("ser.huff", ios::app);
	//if (fp.is_open()) {
	//	fp.seekg(0, ios::end);
	//	size1 = fp.tellg();
	//	fp.close();
	//}
	//else {
	//	cerr << "这文件打不开！！！！";
	//	exit(-1);
	//}
	//cout << "huffman压缩后ser.huff大小为：" << size1 << "字节。" << endl;
	//cout << "压缩比为：" << 100.00*size1 / (double)size0 << "%。" << endl << endl;

	cout << "************************************************************************" << endl;
	cout << "                           以下演示lz77压缩                             " << endl;

	ta = clock();
	lz77("ser.log", "ser.lz77");
	cout << "lz77压缩成功！" << endl;
	tb = clock();
	cout << "lz77压缩文件用时：" << (t1 = ((double)tb - (double)ta)) / CLOCKS_PER_SEC << "秒" << endl;
	ta = clock();
	delz77("ser.lz77", "ser.v2");
	tb = clock();
	cout << "lz77解压文件用时：" << (t3 = ((double)tb - (double)ta)) / CLOCKS_PER_SEC << "秒" << endl;
	cout << "------------------------------------------------" << endl;
	fp.open("ser.lz77", ios::app);
	if (fp.is_open()) {
		fp.seekg(0, ios::end);
		size1 = fp.tellg();
		fp.close();
	}
	else {
		cerr << "这文件打不开！！！！";
		exit(-1);
	}
	cout << "lz77压缩后ser.lz77大小为：" << size1 << "字节。" << endl;
	cout << "压缩比为：" << 100.00*size1 / (double)size0 << "%。" << endl << endl;

	return 0;
}