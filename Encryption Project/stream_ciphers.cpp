#include <iostream>
#include <string>
#include <cmath>

char *encode( char *plaintext, unsigned long key );
char *decode( char *cihpertext, unsigned long key );

#ifndef MARMOSET_TESTING
int main() {

	char test[]{ "hello ECE 150 class" };

	char *ciphertext{encode(test, 150)};

	char *plaintext{decode(ciphertext, 150)};

	return 0;

}
#endif

char *encode(char *plaintext, unsigned long key ){
	//Getting size of length
	unsigned int size = 0;
	unsigned increment = 0;
	while(plaintext[increment] != '\0'){
		size++;
		increment++;
	}

	//std::cout<<"Size: "<<size<<std::endl;

	char *temparray = plaintext;
	unsigned int null = 4 - (size % 4);

	if(size % 4 == 0){
		null = 0;
	}
	else{
		null = 4 - (size % 4);
	}

	//std::cout<<"Null: "<<null<<std::endl;

	plaintext = {new char[size+null]};

	for(int i = 0; i < size; i++){
		plaintext[i] = temparray[i];
	}
	for(int i = 0; i < null; i++){
		plaintext[size + i] = '\0';
	}

	//Scrambled Array
	unsigned char S[256]{};

	for(unsigned int i = 0; i<256; i++){
		S[i] = i;
	}

	unsigned int j = 0;
	unsigned int i = 0;

	for(unsigned int count = 0; count<256; count++){
		unsigned int k = i % 64;
		j = (j + S[i] + (1 & (key >> k))) % 256;
		unsigned char temp = S[i];
		S[i] = S[j];
		S[j] = temp;
		i = (i % 256) + 1;
		//std::cout<<S[i]<<std::endl;
	}

	//This is for testing
	/*for(int i = 0; i < 256; i++){
		std::cout<<S[i]<<std::endl;
	}*/

	//std::cout<<"Size: "<<size<<std::endl;

	//unsigned char temparray[size]{};

	for(unsigned int count = 0; count<(size+null); count++){
		i = (i + 1) % 256;
		j = (j + S[i]) % 256;
		unsigned char temp = S[i];
		S[i] = S[j];
		S[j] = temp;
		unsigned long r = (S[i] + S[j]) % 256;
		unsigned long R = S[r];
		plaintext[count] = static_cast<unsigned char>(R ^ plaintext[count]);

		//std::cout<<"R: "<<R<<", New char: "<<plaintext[count]<<std::endl;
	}

	//For Testing
	/*for(int i = 0; i < size; i++){
		std::cout<<plaintext[i];
		}
	std::cout<<std::endl;*/

	unsigned int range = (size + null);
	char finalchars[int(range * 1.25)]{};

	//For Testing
	/*for(unsigned int count = 0; count < range; count++){
		unsigned char test = int(plaintext[count]);
		std::cout<<"R: "<<int(test)<<", New char: "<<plaintext[count]<<std::endl;
	}*/

	//std::cout<<"Range: "<<range<<std::endl;

	unsigned int increase = 0;

	//Ascii Armour
	for(unsigned int count = 0; count < range; count += 4){
		unsigned char letter1 = int(plaintext[count]);
		unsigned char letter2 = int(plaintext[count+1]);
		unsigned char letter3 = int(plaintext[count+2]);
		unsigned char letter4 = int(plaintext[count+3]);

		unsigned int total = (int(letter1) << 24) + (int(letter2) << 16) + (int(letter3) << 8) + (int(letter4) << 0);
		//std::cout<<total<<std::endl;

		for(unsigned int terms = 0; terms < 5; terms++){
			unsigned int rem = total % 85;
			total = (total - total % 85)/85;
			finalchars[(4-terms)+count+increase] = char(rem + 33);
			//std::cout<<finalchars[(4-terms)+count+increase];
		}
		increase++;
		//std::cout<<std::endl;
	}

	char *ciphertext = new char[int((range*1.25) + 1)];

	//For Testing
	for(unsigned int count = 0; count<int(range*1.25); count++){
		ciphertext[count] = finalchars[count];
		std::cout<<ciphertext[count];
	}
	std::cout<<std::endl;

	ciphertext[int(range*1.25)] = '\0';

	return ciphertext;

}

char *decode( char *ciphertext, unsigned long key ){
	unsigned int size = 0;
	unsigned int increment = 0;

	while(ciphertext[increment] != '\0'){
		//std::cout<<"Cipher: "<<ciphertext[increment]<<std::endl;
		size++;
		increment++;
	}

	//std::cout<<std::endl;

	unsigned int limit = int(size/5);
	unsigned int newrange = int(size*0.8);

	char *temparray = ciphertext;
	char characterarray[newrange + 1];

	//std::cout<<"Size: "<<size<<std::endl;

	for(unsigned int count = 0; count < limit; count++){
		unsigned int sum = 0;
		for(unsigned int iteration = 0; iteration < 5; iteration++){
			unsigned char value = static_cast<unsigned char>((temparray[(count*5)+iteration])-33);
			//total += int(value) * pow(85, 4-iteration);
			sum += int(value) * pow(85, 4-iteration);
			//std::cout<<"Value ("<<(count*5)+iteration<<"): "<<int(value)<<std::endl;
		}

		for(int iteration = 0; iteration < 4; iteration++){
			unsigned int rem = sum % 256;
			sum = (sum-rem)/256;
			unsigned char value = char(rem);
			characterarray[(3-iteration) + (count*4)] = value;
			//std::cout<<value;
		}
	}
	//std::cout<<std::endl;

	//For Testing
	/*for(int count = 0; count < newrange; count++){
		std::cout<<characterarray[count];
	}*/

	unsigned char S[256]{};

	for(unsigned int i = 0; i<256; i++){
		S[i] = i;
	}

	unsigned int j = 0;
	unsigned int i = 0;

	for(unsigned int count = 0; count<256; count++){
		unsigned int k = i % 64;
		j = (j + S[i] + (1 & (key >> k))) % 256;
		unsigned char temp = S[i];
		S[i] = S[j];
		S[j] = temp;
		i = (i % 256) + 1;
		//std::cout<<S[i]<<std::endl;
	}

	for(unsigned int count = 0; count<newrange; count++){
			i = (i + 1) % 256;
			j = (j + S[i]) % 256;
			unsigned char temp = S[i];
			S[i] = S[j];
			S[j] = temp;
			unsigned long r = (S[i] + S[j]) % 256;
			unsigned long R = S[r];
			characterarray[count] = static_cast<unsigned char>(R ^ characterarray[count]);

			std::cout<<"R: "<<R<<", New char: "<<characterarray[count]<<std::endl;
		}

	unsigned int newsize = 0;
	unsigned int newincrement = 0;

	while(characterarray[newincrement] != '\0'){
		newsize++;
		newincrement++;
	}

	char *plaintext = new char[newrange + 1];

	//std::cout<<"New Range: "<<newrange<<std::endl;

	//For Testing
	for(unsigned int count = 0; count < sizeof(characterarray); count++){
		plaintext[count] = characterarray[count];
		std::cout<<plaintext[count];
	}

	return plaintext;
}

