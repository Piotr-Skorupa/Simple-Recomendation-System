#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <iomanip>
#include <algorithm>


#define H "horror"
#define K "komedia"
#define S "sensacyjny"
#define R "romans"


struct Movie
{
	std::string title;
	std::string category;
	int rate;

	Movie(const char* t, const char* c, int r=0): title(t), category(c), rate(r)
	{
	}

};

struct User
{
	std::vector<bool> obejzane;   // watched films
	std::vector<int> oceny;            // rates

	User(std::vector<Movie> baza) : obejzane(baza.size(), false), oceny(baza.size(), 0)
	{
	}

};



void update_movies(std::vector<Movie> & vec)
{
	vec.clear();
	vec.emplace_back(Movie("Koszmar z Ulicy Wiazow", H));
	vec.emplace_back(Movie("Rambo", S));
	vec.emplace_back(Movie("Red", S));
	vec.emplace_back(Movie("Pachnidlo", R));
	vec.emplace_back(Movie("Martwe Zlo", H));
	vec.emplace_back(Movie("Sasiedzi", K));
	vec.emplace_back(Movie("Sasiedzi 2", K));
	vec.emplace_back(Movie("Kac Vegas w Bangkoku", K));
	vec.emplace_back(Movie("Zywot Briana", K));
	vec.emplace_back(Movie("Piekna i Bestia", R));
	vec.emplace_back(Movie("Rec", H));
	vec.emplace_back(Movie("Autopsja Jane Doe", H));
	vec.emplace_back(Movie("La La Land", R));
	vec.emplace_back(Movie("Kochane Zycie", R));
	vec.emplace_back(Movie("Sms do Ciebie", R));
	vec.emplace_back(Movie("Od teraz na Zawsze", R));
	vec.emplace_back(Movie("Spojrz Mi w Oczy", R));
	vec.emplace_back(Movie("Don't Knock Twice", H));
	vec.emplace_back(Movie("Mama", H));
	vec.emplace_back(Movie("Get Out!", H));
	vec.emplace_back(Movie("Krucyfiks", H));
	vec.emplace_back(Movie("Predator", S));
	vec.emplace_back(Movie("Obcy 2", S));
	vec.emplace_back(Movie("Deadpool", S));
	vec.emplace_back(Movie("Avengers", S));
	vec.emplace_back(Movie("Co ty wiesz o swoim dziadku?", K));
	vec.emplace_back(Movie("Sausage Party", K));
	vec.emplace_back(Movie("21 Jump Street", K));
	vec.emplace_back(Movie("Rekiny Wojny", K));
}


void wypisz_filmy( const std::vector<Movie> & vec, std::vector<bool> ob)
{
	// print movies in console
	std::cout << "\n";
	for (auto i = 0; i < vec.size(); i++)
	{
		std::string obejzane;
		(ob[i] == true) ? obejzane = "obejzane" : obejzane = "";   // is watched ?
		std::cout << std::fixed;
		std::cout << i <<". ";
		std::cout << std::left << std::setw(28) << vec[i].title;
		std::cout << std::right << std::setw(28) << ": "; 
		std::cout << vec[i].category << " ----> " << obejzane;
		(ob[i] == true) ? std::cout << " " << vec[i].rate << std::endl : std::cout << std::endl;
	}
}


double podobienstwo(User const& u1, User const& u2)
{
	double res = 0.0;
	int ilosc_obejzanych = 0;
	for (auto i = 0; i < u1.obejzane.size(); i++)
	{
		if (u1.obejzane[i] == true && u2.obejzane[i] == true)
		{
			ilosc_obejzanych++;
			res += std::pow(u1.oceny[i] - u2.oceny[i], 2); // wyzanaczanie odleglosci
														  // euklidesowej pomiedzy ocenami
		}

	}
	if (ilosc_obejzanych == 0)
		return res;

	res = std::sqrt(res / ilosc_obejzanych);
	res = 1.0 - std::tanh(res);       // tangens hiperboliczny

	return res;
}





void rekonendacje(const std::vector<Movie> & vec, User & u1, User & u2, User & u3)
{
	static int step = 0;
	std::random_device rd{};
	static std::mt19937 gen{rd()};
	static std::uniform_int_distribution<int> dis(0, vec.size() - 1);
	int random = 0;
	std::vector<Movie> reko;
	if (step == 0) {
		for (int i = 0; i < 3; i++)
		{
			random = dis(gen);
			while (u1.obejzane[random]!=false)
			{
				random = dis(gen);
			}
			reko.emplace_back(vec[random]);
		}
		++step;

		std::cout << "\nPOLECANE: " << std::endl;
		for (auto const& r : reko) {
		std::cout <<"'"<< r.title << "'  " << r.category << std::endl;
		}
	}
	else {
		
		// MIEJSCE NA INTELIGENTNY ALGORYTM REKOMENDACJI
		
		double suma_podobienstw = 0.0;
		double suma_wag = 0.0;

		suma_podobienstw = podobienstwo(u1, u2) + podobienstwo(u1,u3);

		std::vector<std::pair<int, double>> klucze_filmow;

		for (auto i = 0; i < vec.size(); i++)
		{
			suma_wag = 0.0;
			if (vec[i].rate == 0 && (u2.obejzane[i] == true || u3.obejzane[i] == true)) {
				klucze_filmow.push_back(std::pair<int, double>(i, 0.0));
				if (u2.obejzane[i] == true) {
					suma_wag += u2.oceny[i] * podobienstwo(u1, u2);
				}
				if (u3.obejzane[i] == true) {
					suma_wag += u3.oceny[i] * podobienstwo(u1, u3);
				}

				if (suma_podobienstw > 0.0) {
					klucze_filmow.back().second = suma_wag / suma_podobienstw;
				}
		

			}
		}
		
		std::sort(klucze_filmow.begin(), klucze_filmow.end(), [](auto &left, auto &right) {
			return left.second > right.second; });

		std::cout << "\nPOLECANE: " << std::endl;

		for (auto & k : klucze_filmow)
		{
			std::cout << k.first << ". " << vec[k.first].title << ", prognoza: "<<  k.second << std::endl;
		}


		//do tad
		++step;
	}
	

}


int main()
{
	int choice, choice2;
	   
	std::vector<Movie> baza_filmow;
	update_movies(baza_filmow);

	// creating 3 users
	User ja(baza_filmow);
	User ty(baza_filmow);
	User trzeci(baza_filmow);

	trzeci.obejzane[0] = true;
	trzeci.obejzane[1] = true;
	trzeci.obejzane[2] = true;
	trzeci.obejzane[3] = true;
	trzeci.obejzane[4] = true;
	trzeci.oceny[0] = 3;
	trzeci.oceny[1] = 3;
	trzeci.oceny[2] = 3;
	trzeci.oceny[3] = 3;
	trzeci.oceny[4] = 3;
	
	do {
		system("cls");
		wypisz_filmy(baza_filmow, ja.obejzane);
		rekonendacje(baza_filmow, ja, ty, trzeci);

		std::cout << "\nWyliczone podobienstwo z userem 2: " << podobienstwo(ja, ty) << std::endl;
		std::cout << "\nWyliczone podobienstwo z userem 3: " << podobienstwo(ja, trzeci) << std::endl;

		std::cout << "\nJaki film chcesz obejzec ? (666 to exit)" << std::endl;
		
		std::cin >> choice;
		if (choice >= ja.obejzane.size()) continue;
		ja.obejzane[choice] = true;

		std::cout << "Ocen ten film (od 1 do 5): " << std::endl;
		std::cin >> baza_filmow[choice].rate;
		ja.oceny[choice] = baza_filmow[choice].rate;

		int zliczanie_obejzanych = 0;
		for (auto &o : ja.obejzane) {
			if (o == true) zliczanie_obejzanych++;
		}
		
		if (zliczanie_obejzanych == ja.obejzane.size()) choice = 666;

		std::cout << "Jaki film oglada drugi uzytkownik? : " << std::endl;
		std::cin >> choice2;
		std::cout << "Ocena: " << std::endl;
		std::cin >> ty.oceny[choice2];
		ty.obejzane[choice2] = true;

		

	} while (choice != 666);

	std::cout << "\nKONIEC!";

	std::cin.get();
	std::cin.get();
	return 0;
}








