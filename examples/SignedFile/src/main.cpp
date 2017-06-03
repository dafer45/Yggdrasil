#include "IggStream.h"

#include <fstream>
#include <iostream>

using namespace std;
using namespace Ygg;

int main(int argc, char **argv){
	IggStream iggStream;
	iggStream.setStore("File");
	iggStream.setResource("makefile");
	iggStream.setSignature(
		"kRiZ8M7bSdwEz82fPEa39iJL3bTCamA1HotUfbzevJpBkMTTmByfuiLiasbGpkWR"
		"kOypnXC1FlJOsY0swjgh2otpdoKi2Zo2E39m0pOAEJFQMpHAJMAbAdJQjUWw7kHc"
		"R6yDOmqPHahUFbEDBimPJUW5vWUUsHuHYnTk+xMJrUCFRHG7bGQxCSPDFzSYaOo5"
		"cTVlHA4+iVyqgqRhD5XMwmIepeabrGg2sR6gyZ75w9JEifzUPhd0yk3pHP2yg2ve"
		"+Enu+mfpxnJ2YJCM89vMENMMTOPvWYTD/NcXk0O2EwtPANSxTtxEhbhDquNSRsoi"
		"duxX0z1JVk8PcIuVHnKZz8hiZuncdKLYQt5DuHBuX+okJHettGyW6z45e1CInjfY"
		"o3RST3tYjmWfb62vYBMauEyNLxbaaWzw9TTUSZSb+JcnVwXalO5vWS/nAM8WZ4QT"
		"3tJlCfygoMbwgMl+qA5cKp6RK8vKMlhWUJi1kqgROnhOSzEsbnjXTw9+bRMl6MVX"
		"9TDgMXiKjOEcLkteF72FSBzK/TZ2HU7bWO7UOjWZzlU/W9/jw2XajN62O5p27C+C"
		"aISEFlQsDDJFXDfA2maLj3/QImaKGAHc11oPOhDZ4z6BhxPMfKb+Pza5u6+QERSG"
		"DSyumjz5KMGcTCSKiBaWQt6bOj3rXW+MIAXv5uSrLcA="
	);
	iggStream.open();

	while(!iggStream.eof()){
		string line;
		getline(iggStream, line);
		cout << line << "\n";
	}

	return 0;
}