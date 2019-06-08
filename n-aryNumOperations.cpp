/*
** Program : To perform operations like add, subtract, multiply, divide with very long numbers with bases both +ve and -ve
** Programmer : Ajay Pratap Singh (cs12b033)
** Date : 9 June 2019, 1:06am IST
** Email : 
** TODO: Some things are incomplete, multiplication and subtraction will be impletented and updated. 
** Notes: Feel free to use the code, just remember to cite my work in your publication/submission.
*/
#include <bits/stdc++.h> 

using namespace std;

#define debug 0
#if debug
	#define DP if(1)
#else
	#define DP if(0)
#endif


const string DIGITS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

// These maps support data for multiple base encoding and decoding to and from base 10 to base 'b'
map<int, map<int64_t, string> > encodeMap;
map<int, map<string, int64_t> > decodeMap;

void print2DMap(map<auto, map<auto, auto> > mp)
{
	cout << "MAP {" << endl;
	for (auto itr = mp.begin(); itr != mp.end(); itr++) 
	{
        cout << "\t\t" << itr->first << " : (" << endl;
        for (auto itr2 = itr->second.begin(); itr2 != itr->second.end(); itr2++) 
        {
        	cout << "\t" << itr2->first << " -> " << itr2->second << endl;
        }
        cout << "\t\t)," << endl << endl; 
	}
     cout << "}" << endl;
}

string truncateZeros(string ns)
{
	int i = 0;
	int N = ns.length();
	while(i < N)
	{
		if(ns[i] == '0')
		{
			i++;
		}
		else
		{
			break;
		}
	}
	string res = ns.substr(i, N - i);
	if(res == "")
	{
		return "0";
	}
	else
	{
		return res;
	}
}

/*
** This function encodes a number (provided in base 10) and returns string in base 'b'
*/
string encode(int64_t n, int b) {
	// Checking encoded values from map
	map<int, map<int64_t, string> >::iterator it = encodeMap.find(b);
	if(it != encodeMap.end())
	{
		map<int64_t, string>::iterator it2 = it->second.find(n);
		if(it2 != it->second.end())
		{
			return it2->second;
		}	
	}

	if (b < -62 || b > 62 || b == 0) {
		throw runtime_error("Argument out of range: 'base' must be [-62, 62] - {0}");
	}
	if (n == 0) {
		return "0";
	}

	string output;
	int64_t nn = n;
	while (nn != 0) {
		int rem = nn % b;
		nn /= b;
		if (rem < 0) {
			nn++;
			rem -= b;
		}
		output += DIGITS[rem];
	}
 
	reverse(output.begin(), output.end());
	encodeMap[b][n] = output;
	return output;
}
 
/*
** This function decodes a number given in base 'b' and returns interger in base 10
*/
int64_t decode(const string& ns, int b) {
	// Checking decode value from map
	map<int, map<string, int64_t> >::iterator it = decodeMap.find(b);
	if(it != decodeMap.end())
	{
		map<string, int64_t>::iterator it2 = it->second.find(ns);
		if(it2 != it->second.end())
		{
			return it2->second;
		}	
	}

	if (b < -62 || b > 62 || b == 0) {
		throw runtime_error("Argument out of range: 'base' must be [-62, 62] - {0}");
	}
	if (ns == "0") {
		return 0;
	}
 
	int64_t total = 0;
	int64_t bb = 1;
 
	for (auto it = ns.crbegin(); it != ns.crend(); it = next(it)) {
		auto ptr = find(DIGITS.cbegin(), DIGITS.cend(), *it);
		if (ptr != DIGITS.cend()) {
			auto idx = ptr - DIGITS.cbegin();
			total += idx * bb;
		}
		bb *= b;
	}
	decodeMap[b][ns] = total;
	return total;
}

/*
** This function converts a string in base prevBase 'b1' and returns string in base newBase 'b2'
*/
string convertBases(string ns, int b1, int b2) {
	string output;
	
	// Method 1: Using encode and decode functions defined above. The limitation is the size of int64_t that can store value.
	// But since int64_t is of 64 bytes and |b| < 63, it will not break for our case. 
	// It will cause issue when we want bases above 62. We will need to implement Method 2 for it and define new symbols for it maybe.
	// Research TODO.
	/*if (b1 < -62 || b1 > 62 || b1 == 0) {
		throw runtime_error("Argument out of range: 'base' must be [-62, 62] - {0}");
	}
	if (b2 < -62 || b2 > 62 || b2 == 0) {
		throw runtime_error("Argument out of range: 'base' must be [-62, 62] - {0}");
	}
	if (ns == "0") {
		return "0";
	}*/
	/*printMap(encodeMap);
	printMap(decodeMap);
	cout << b1 << ", " << b2 << endl;
	cout << "#: " << ns << endl;*/
	output = encode(decode(ns, b1), b2);
	/*cout << "@: " << output << endl;*/
	// Method 2: Using bit by bit conversion from b1 to b2;
	//TODO: 

	return output;
}

/*
** This function adds two numbers (encoded in base)
*/
string addNumbersOfBaseB(string num1, string num2, int base=10) 
{
	num1 = truncateZeros(num1);
	num2 = truncateZeros(num2);
	int len1 = num1.length();
	int len2 = num2.length();
	if(len1 < len2)
	{
		return addNumbersOfBaseB(num2, num1);
	}
	else
	{
		string res = "";
		int i = len1 - 1;
		int j = len2 - 1;
		string sum = "";
		string carry = "";
		string val = "";
		while(i >= 0)
		{
			if(j >= 0)
			{
				sum = encode(decode(string (1, num1[i]), base) + decode(string (1, num2[j]), base), base);
				sum = encode(decode(sum, base) + decode(carry, base), base);
			}
			else
			{
				sum = encode(decode(string(1, num1[i]), base) + decode(carry, base), base);
			}
			int tempLen = sum.length();
			val = sum[tempLen - 1];
			carry = sum.substr(0, tempLen - 1);
			
			DP cout << "#: " << i << ", " << j << " : " << sum << ", " << carry << ", " << val << endl;
			res += val;
			DP cout << res << endl;
			i--;
			j--;
		}
		if(carry != "0")
		{
			DP cout << "$: " << carry << endl;
			res += carry;
		}
		reverse(res.begin(), res.end());
		return truncateZeros(res);
	}
}

string sub(string p, string q, int b, string &burrow)
{
	/*if(b > 0)*/
	{
		int firstInt = decode(p, b);
		int secondInt = decode(q, b);
		int burrowVal = decode(burrow, b);
		firstInt -= burrowVal;
		if(firstInt < secondInt)
		{
			firstInt = firstInt + b;
			burrow = "1";
		}
		else
		{
			burrow = "";
		}
		return encode(firstInt - secondInt, b);
	}
	/*else
	{
		// since negative numbers can be reprented using -ve base, burrow is not needed in this case
		return encode(decode(p, b) - decode(q, b), b);
	}*/
}

/*
** This function subtract two numbers (encoded in base)
*/
string subtractNumbersOfBaseB(string num1, string num2, int base=10) 
{
	// TODO: Need to put more thoughts
	// Length does not represent value of numbers in -ve base. Check value by decoding. TODO
	num1 = truncateZeros(num1);
	num2 = truncateZeros(num2);
	int len1 = num1.length();
	int len2 = num2.length();
	if(len1 < len2)
	{
		//return subtractNumbersOfBaseB(num2, num1);
		throw runtime_error("Invalid Operations. Larger number cannot be substracted from smaller number.");
	}
	else
	{
		string res = "";
		int i = len1 - 1;
		int j = len2 - 1;
		string total = "";
		string carry = "";
		string burrow = "";
		string prevBurrow = "";
		string val = "";		
		string s1;
		string s2;
		while(i >= 0)
		{
			s1 = string(1, num1[i]);
			string enc1 = encode(decode(s1, base) + decode(carry, base), base);
			if(j >= 0)
			{
				s2 = string(1, num2[j]);
			}
			else
			{
				s2 = "";
			}

			total = sub(enc1, s2, base, burrow);
			int tempLen = total.length();
			val = total[tempLen - 1];
			carry = total.substr(0, tempLen - 1);			
			DP cout << "#: " << i << ", " << j << " : " << total << ", " << carry << ", " << val << endl;
			res += val;
			DP cout << res << endl;
			i--;
			j--;
		}
		if(carry != "0")
		{
			DP cout << "$: " << carry << endl;
			res += carry;
		}
		reverse(res.begin(), res.end());
		return truncateZeros(res);
	}
}

/*
** This function multiplies two numbers (encoded in base)
*/
string multiplyNumbersOfBaseB(string num1, string num2, int base=10) 
{
	// TODO: Need to put more thoughts
	throw runtime_error("Multiplication: NOT implemented yet!");
	int len1 = num1.length();
	int len2 = num2.length();
	/*if(len1 < len2)
	{
		// TODO : have to think about implications of lengths in bit multiplication
		return subtractNumbersOfBaseB(num2, num1);
	}
	else*/
	{
		string res = "";
		int i = len1 - 1;
		int j = len2 - 1;
		string sum = "";
		string carry = "";
		string val = "";
		while(i >= 0)
		{
			if(j >= 0)
			{
				sum = encode(decode(string (1, num1[i]), base) * decode(string (1, num2[j]), base), base);
				sum = encode(decode(sum, base) * decode(carry, base), base);
			}
			else
			{
				sum = encode(decode(string(1, num1[i]), base) * decode(carry, base), base);
			}
			int tempLen = sum.length();
			val = sum[tempLen - 1];
			carry = sum.substr(0, tempLen - 1);
			
			DP cout << "#: " << i << ", " << j << " : " << sum << ", " << carry << ", " << val << endl;
			res += val;
			DP cout << res << endl;
			i--;
			j--;
		}
		if(carry != "0")
		{
			DP cout << "$: " << carry << endl;
			res += carry;
		}
		reverse(res.begin(), res.end());
		return res;
	}
}

/*
** This function divides two numbers (encoded in base)
*/
string divideNumbersOfBaseB(string num1, string num2, int base=10) 
{
	// TODO: check for the floating points and loss in conversion.  Also division by zero
	throw runtime_error("Division: NOT implemented yet!");
	int len1 = num1.length();
	int len2 = num2.length();
	if(len1 < len2)
	{
		/*return subtractNumbersOfBaseB(num2, num1);*/
		cout << "Decimal result expected. TODO" << endl;
		return "";
	}
	else
	{
		string res = "";
		int i = len1 - 1;
		int j = len2 - 1;
		string sum = "";
		string carry = "";
		string val = "";
		while(i >= 0)
		{
			if(j >= 0)
			{
				sum = encode(decode(string (1, num1[i]), base) / decode(string (1, num2[j]), base), base);
				sum = encode(decode(sum, base) / decode(carry, base), base);
			}
			else
			{
				sum = encode(decode(string(1, num1[i]), base) / decode(carry, base), base);
			}
			int tempLen = sum.length();
			val = sum[tempLen - 1];
			carry = sum.substr(0, tempLen - 1);
			
			DP cout << "#: " << i << ", " << j << " : " << sum << ", " << carry << ", " << val << endl;
			res += val;
			DP cout << res << endl;
			i--;
			j--;
		}
		if(carry != "0")
		{
			DP cout << "$: " << carry << endl;
			res += carry;
		}
		reverse(res.begin(), res.end());
		return res;
	}
}

void initialize(int &n, int &b, string &exp, string &enc, int64_t &dec, string &num, int &b1, int &b2, string &exp2, string &num1, string &num2, string &exp3, string &exp4, string &exp5, string &exp6)
{
	/*//Testcase #1
	n = 15;
	b = 2;
	exp = "1111";

	num = "120";
	b1 = 3;
	b2 = 2;
	exp2 = "1111";*/

	/*// Testcase #2
	n = 15;
	b = -2;
	exp = "10011";

	num = "120";
	b1 = 3;
	b2 = 2;
	exp2 = "1111";*/

	/*// Testcase #3
	n = 62;
	b = 63;
	exp = "14";

	num = "120";
	b1 = 3;
	b2 = 2;
	exp2 = "1111";*/

	/*// Testcase #4
	num1 = "10011";
	num2 = "101";
	b = -2;
	exp3 = "10100";*/
	
	/*// Testcase #5
	num1 = "100111101010101001100101011110100101111111111101000000001110111001100001011101100101100100101011111010111010100101101110111110010011111010101100011000000101010010001111101010000101110001100111000010010011000110101010001001101111100101100000000110011110001001100010011000010011111110110111000101101001010101100101010100110110001010011000101000010000010111100000100011011101010010111100000111111101111000000111100101110011000100000010111110001101101011001000100100010000010010011100110000110011011000000111010110110001011100011011000100100101100100101011111000010101000010110110111010101001000100100000010011100010001100011000110100010111000000110100110101000000100010000001101111111111011110001101101111100011010100000010100110001101000111000010101111101111110110011000011010010000111110110010010000100010101110101110010110010100111000000010111111010101011000111000011001110101110010110001001111001000011110011010011111100101101001001111101001101101101110010110101111110111111111101101010010110000110";
	num2 = "110100110110001011111101100110100110110001011001110110101000011011100011100101100000100001000110111111001101100110001010101011010111000101100000111111110100000010000110011110000001100000000001111000000100010010000101101000111010111100001010111011110111001111100000000010001001100101111100011011010000101000100110101001010010100111101111100100000111011111000010111010110000000011010011011000110011011111100001000011010110111110010100000110000010001001011100010001001011000101010100111111100010101010101110001110111101100000001011001011000100011111000010100101010100100010001001100111101100101110110001110110011110100001101011100001010110111000010101111010010111010110111000001011010001000000011110101001111101100010001011100110011001000110000001100001010110100010011001101100010100110011110011000001110000000011011011110100010111111101010011011010101001011111110011010000000001111000100111011100001010000011010101100000001010010010111011111011101001001101101110101000000100010101111001111001001000011";
	b = -2;
	exp3 = "";*/

	// Tescase #6 (Overall testing)
	n = 15;
	b = 2;
	exp = "1111";
	num = exp;
	b1 = b;
	b2 = -2;
	exp2 = "10011";

	num1 =    "11100";
	num2 =      "111";
	exp3 =   "100011";
	exp4 =    "10101";
	exp5 = "11000100";
	exp6 =      "100";

	/*// Testcase #7 (convertBases testing with conversion from base 2 to -2)
	num = "1111";
	b1 = 2;
	b2 = -2;
	exp2 = "10011";*/

	/*// Testcase #8 
	num1 = "10011";
	num2 =   "101";
	exp4 = "11110";
	b = -2;
	exp = exp4;*/

	/*// Testcase #8a 
	num1 = "1000011";
	num2 =     "101";
	exp4 = "1001110";
	b = -2;*/

	/*// Testcase #8b
	b = 2;
	num1 = "100010";
	num2 =    "100";
	exp4 =  "11110";*/

	/*// Testcase #8c
	b = 2;
	num1 =   "101";
	num2 =    "10";
	exp4 =    "11";*/

	/*// Testcase #8d
	b = -5;
	num1 = "242";
	num2 = "134";
	exp4 = "123";*/

	/*// Testcase #8e
	b = -5;
	num1 = "40000";
	num2 =   "314";
	exp4 = "41201";*/

	// Testcase #9
	/*b = -5;
	//n = 46;
	num1 = "242";
	num2 = "134";
	exp3 = "211";
	exp4 = "123";*/

	/*// Testcase #9
	b = -5;
	exp = "12";
	n = -5;*/


	return;
}

int main() {
	 
	int n;
	int b;
	string exp;
	string enc;
	int64_t dec;
	string num;
	int b1;
	int b2;
	string exp2;
	string num1;
	string num2;
	string exp3;
	string exp4;
	string exp5;
	string exp6;

	initialize(n, b, exp, enc, dec, num, b1, b2, exp2, num1, num2, exp3, exp4, exp5, exp6);

	enc = encode(n, b);
	if(enc == exp)
	{
		cout << "SUCCESS: " << n << " in base " << b << " : " << enc << endl; 
	}
	else
	{
		cout << "FAILURE (encoding) : " << enc << ", exp: " << exp << endl;
	}

	dec = decode(exp, b);
	if(dec == n)
	{
		cout << "SUCCESS: " << exp << " decoded from base " << b << " : " << dec << endl;
	}
	else
	{
		cout << "FAILURE (decoding) : " << dec << ", exp: " << n << endl;
	}

	enc = convertBases(num, b1, b2);
	if(enc == exp2)
	{
		cout << "SUCCESS: " << num << " converted from base " << b1 << " to base " << b2 << " : " << enc << endl; 
	}
	else
	{
		cout << "FAILURE (convertBases) : " << enc << ", exp: " << exp2 << endl;
	}

	enc = addNumbersOfBaseB(num1, num2, b);
	if(enc == exp3)
	{
		cout << "SUCCESS: " << num1 << " + " << num2 << " in base " << b << " : " << enc << endl; 
	}
	else
	{
		cout << "FAILURE (addNumbersOfBaseB) : " << enc << ", exp: " << exp3 << endl;
	}

	enc = subtractNumbersOfBaseB(num1, num2, b);
	if(enc == exp4)
	{
		cout << "SUCCESS: " << num1 << " - " << num2 << " in base " << b << " : " << enc << endl; 
	}
	else
	{
		cout << "FAILURE (subtractNumbersOfBaseB) : " << enc << ", exp: " << exp4 << endl;
	}

	enc = multiplyNumbersOfBaseB(num1, num2, b);
	if(enc == exp5)
	{
		cout << "SUCCESS: " << num1 << " * " << num2 << " in base " << b << " : " << enc << endl; 
	}
	else
	{
		cout << "FAILURE (multiplyNumbersOfBaseB) : " << enc << ", exp: " << exp5 << endl;
	}

	enc = divideNumbersOfBaseB(num1, num2, b);
	if(enc == exp6)
	{
		cout << "SUCCESS: " << num1 << " / " << num2 << " in base " << b << " : " << enc << endl; 
	}
	else
	{
		cout << "FAILURE (divideNumbersOfBaseB) : " << enc << ", exp: " << exp6 << endl;
	}

	return 0;
}