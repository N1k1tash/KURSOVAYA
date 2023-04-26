#include <UnitTest++/UnitTest++.h>
#include "Client.h"

using namespace std;

struct Cons_fix {
	Client * p;
	Cons_fix() {
		p = new Client();
	}
	~Cons_fix() {
		delete p;
	}
};

SUITE(interactWithServer)
{
    TEST_FIXTURE(Cons_fix, SuccessfulScenario){
		p->setServerAddress(2130706433);
        p->setServerPort(33333);
        p->setInputDataFile("/home/user/VSCode/projects1/vectors.bin");
        p->setOutputDataFile("/home/user/VSCode/projects1/resultat.bin");
        p->setClientCredentialsFile("/home/user/VSCode/projects1/config/vclient.conf");
        p->interactWithServer();
		CHECK(true);	
	}
	

    TEST_FIXTURE(Cons_fix, IncorrectIPAddress){
		p->setServerAddress(123456);
        p->setServerPort(33333);
        p->setInputDataFile("/home/user/VSCode/projects1/vectors.bin");
        p->setOutputDataFile("/home/user/VSCode/projects1/resultat.bin");
        p->setClientCredentialsFile("/home/user/VSCode/projects1/config/vclient.conf");
        CHECK_THROW(p->interactWithServer(), ErrorManager);
	}

	TEST_FIXTURE(Cons_fix, IncorrectPortNumber){
		p->setServerAddress(2130706433);
        p->setServerPort(33335);
        p->setInputDataFile("/home/user/VSCode/projects1/vectors.bin");
        p->setOutputDataFile("/home/user/VSCode/projects1/resultat.bin");
        p->setClientCredentialsFile("/home/user/VSCode/projects1/config/vclient.conf");
        CHECK_THROW(p->interactWithServer(), ErrorManager);
	}

	TEST_FIXTURE(Cons_fix, ErrorOpeningLoginAndPasswordFile){
		p->setServerAddress(2130706433);
        p->setServerPort(33333);
        p->setInputDataFile("/home/user/VSCode/projects1/vectors.bin");
        p->setOutputDataFile("/home/user/VSCode/projects1/resultat.bin");
        p->setClientCredentialsFile("/home/user/VSCode/projects1/config/qwe//vclient.conf");
        CHECK_THROW(p->interactWithServer(), ErrorManager);
	}

	TEST_FIXTURE(Cons_fix, IdentificationError){
		p->setServerAddress(2130706433);
        p->setServerPort(33333);
        p->setInputDataFile("/home/user/VSCode/projects1/vectors.bin");
        p->setOutputDataFile("/home/user/VSCode/projects1/resultat.bin");
        p->setClientCredentialsFile("/home/user/VSCode/projects1/config/IdentificationError.conf");
        CHECK_THROW(p->interactWithServer(), ErrorManager);
	}

	TEST_FIXTURE(Cons_fix, AuthenticationError){
		p->setServerAddress(2130706433);
        p->setServerPort(33333);
        p->setInputDataFile("/home/user/VSCode/projects1/vectors.bin");
        p->setOutputDataFile("/home/user/VSCode/projects1/resultat.bin");
        p->setClientCredentialsFile("/home/user/VSCode/projects1/config/AuthenticationError.conf");
        CHECK_THROW(p->interactWithServer(), ErrorManager);
	}

	TEST_FIXTURE(Cons_fix, ErrorOpeningVectorsFile){
		p->setServerAddress(2130706433);
        p->setServerPort(33333);
        p->setInputDataFile("/home/user/VSCode/projects1/qwer/vector.bin");
        p->setOutputDataFile("/home/user/VSCode/projects1/resultat.bin");
        p->setClientCredentialsFile("/home/user/VSCode/projects1/config/vclient.conf");
        CHECK_THROW(p->interactWithServer(), ErrorManager);
	}

	TEST_FIXTURE(Cons_fix, ErrorReadingFromVectorsFile){
		p->setServerAddress(2130706433);
        p->setServerPort(33333);
        p->setInputDataFile("/home/user/VSCode/projects1/ErrorReadingFromVectorsFile.bin");
        p->setOutputDataFile("/home/user/VSCode/projects1/resultat.bin");
        p->setClientCredentialsFile("/home/user/VSCode/projects1/config/vclient.conf");
        CHECK_THROW(p->interactWithServer(), ErrorManager);
	}

	TEST_FIXTURE(Cons_fix, ErrorOpeningSumOutputFile){
		p->setServerAddress(2130706433);
        p->setServerPort(33333);
        p->setInputDataFile("/home/user/VSCode/projects1/vectors.bin");
        p->setOutputDataFile("/home/user/VSCode/projects1/qwe/resultat.bin");
        p->setClientCredentialsFile("/home/user/VSCode/projects1/config/vclient.conf");
        CHECK_THROW(p->interactWithServer(), ErrorManager);
	}
}

int main(int argc, char **argv)
{
	return UnitTest::RunAllTests();
}

