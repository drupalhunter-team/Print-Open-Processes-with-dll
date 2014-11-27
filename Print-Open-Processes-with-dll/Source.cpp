//Bu programdaki amaç işletim sistemi üzerinde çalişan bütün
//prosesleri ve bunlara ait bütün modülleri yazdirmaktir.
//bir önceki örnekte kodun büyük  bir bölümünü tek tek anlattiğim için
//Bu programda yorum sayisi azaliyor.
//Fonksiyonlarin sonunda A değeri görünce şaşirmayin bu onlarin 8 bitlik karakter 
//sisteminde çaliştiğini gösterir.
//GetProcessNameA örneğinde olduğu gibi

#include <windows.h>
#include <stdio.h>
#include <psapi.h>



#pragma comment(lib,"psapi.lib")

//Proseslerin modül listesini sadece konsole yazmaktansa bir txt dosyasi içerisine yazdiracağiz.
//Aşağidaki global değişken dosya kontrolü için gereklidir.

FILE *File;
void PrintProcessInformation(DWORD ProcessID)
{
	//Proces ismi maksimum 260 karakter alabilir.
    char ProcessName[MAX_PATH] = "<------Bilinmiyor------>";

    // Proces handle değeri aliniyor.
	// Bazi proseslerin handle değeri alinamaz
    HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                                   PROCESS_VM_READ,
                                   FALSE, ProcessID );

    // Prosesin handle değeri alinip alinmadiği kontrol edilir.

    if (hProcess != NULL  )
    {
		//Var olan bütün modülleri alacağimizdan  1024 kapasiteli bir dizi oluşturdum
		//Umalim ki 1024 den fazla modül olmasin :)
        HMODULE hMod[1024]; 

		DWORD NumBytes;
		//Proses'in ilk Modülünü yakala
        if ( EnumProcessModules(hProcess, 
								hMod, 
								sizeof(hMod), //1024 adet hMod olduğunu bu paraametre ile belirtiyoruz. Bu sayede fonksiyon bulduğu bütün modülleri yükleyecektir.
								&NumBytes) )
        {
			
			DWORD NumModule		= NumBytes/sizeof(HMODULE);
			
			//Yakalanan modülün ismi al
			GetModuleBaseNameA( hProcess, hMod[0], ProcessName, 
							   sizeof(ProcessName)/sizeof(char) );
			
			printf("\n----------------------------------------------------------------------\n");
			
			//Aşağidaki fonksiyonu hatirlamadiysaniz açiklayayim. Ayni printf gibi davranir.
			//sadeece ekrana yazacağina dosyaya yazar yazisini.
			//ilk parametre dosyayi kontrol eden değişkendir.
			
			fprintf(File,"\n----------------------------------------------------------------------\n");
			
			printf(ProcessName);
			fprintf(File,ProcessName);
			
			printf("\n----------------------------------------------------------------------\n");
			fprintf(File,"\n----------------------------------------------------------------------\n");
			for(int i = 1;i<NumModule; i++)
			{

				char ModuleName[MAX_PATH] = "isimsiz";
				
				//Yakalanan modülün ismi al
				
				GetModuleBaseNameA( hProcess, hMod[i], ModuleName, 
								   sizeof(ModuleName)/sizeof(char) );
				printf("\n----->");
				fprintf(File,"\n----->");
				printf(ModuleName);
				fprintf(File,ModuleName);
			}
			Sleep(5000);
        }
    }

    CloseHandle( hProcess );

}

int main(int argc,char *argv[])
{
	DWORD	NumBytes		= 0;		
	DWORD	NumProcess		= 0;
	DWORD	ProIndexes[1024];
	
	system("color 4B"); //arka plan rengi için.

	//Aşağidaki fonksiyon "ProsesModulListesi.txt" adindan bir dosya oluşturup kullanima açacaktir.
	//bu dosyaya File değişkeni ile erişeceğiz.
	//   "..\\"  anlami bir önceki klasör demektir. (daha ne kadar derine giderim bilemiyorum)
	// Eğer bir tane \ kullanirsaniz escape karakteri anlamina gelmektedir. bu yüzden iki tane kullaniyoruz.
	// derleyici bir tane \ olarak algilar .
	
	File = fopen("..\\ProsesModulListesi.txt","w");
	
	//Öncelikle ProIndexes dizinini sifirliyoruz
	ZeroMemory(ProIndexes,sizeof(ProIndexes));


	//Sistemdeki bütün processlerin ID değerlerini dizinimize kaydediyoruz
	EnumProcesses(ProIndexes,	//dizinin adresi
				  1024,			//dizinin eleman sayisi
				  &NumBytes);	//Diziye yüklenen bilginin byte olarak büyüklüğü
	
	//Diziye yüklenen bilgi byte olarak gelmiş durumda
	//Her bir bilgi 4byte yani DWORD
	//Bölüm işlemi sonucunda diziye kaç eleman yüklenmiş olduğunu bulacağiz.

	NumProcess = NumBytes/sizeof(DWORD);  

	for(int i=0;i<NumProcess;i++)
	{
		PrintProcessInformation(ProIndexes[i]);
	}
	///dosyayi kontrol eden değişkeni kapatiyoruz
	fclose(File);
}
//Konsolumuzun buffer kisitlandirmasi yüzünden bütün yazilari görememekteyiz.

