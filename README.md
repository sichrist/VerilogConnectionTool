
ConnectionTool/
│	
├─ bin/
│	
├─ cfg/
│	
├─ doc/
│	
├─ flow/
│	
├─ header/
│	
├─ lib/
│	│	
│	├─ inc/ 
│	│	
│	└─ src/
│	
├─ main/
│	
├─ obj/
│	
├─ py/
│	
├─ src/
│	
├─ vpi/
│	│	
│	├─ f9/ 
│	│	
│	├─ x1/
│	│	
│	├─ src/
│	│	
│	└─ generate_modtxt.sh
│   
├─ Makefile
└─ Readme.txt



# Erklärung zu den Verzeichnissen

	* bin

		Dieses Verzeichnis enthält ein Makefile, welches die Targets
		clean, clean_indent.bak und bin enthält.
		clean ruft die Targets clean in den Makefiles der anderen 
		Verzeichnisse auf ( main, src und lib/), gleiches gilt für clean_indent.bak.
		Das Target bin ist das default target und baut das ganze Projekt, 
		woraufhin eine ausführbare datei ict erstellt wird.

	* cfg

		Dieses Verzeichnis enthält die Konfigurationsdatei, welche in etwa so aussieht:

		## path to the file which contains the hierarchy
		[MODTXT]: /home/projects/simon/f9/simon_proj/Testprogramm/tests/testfiles/mod.txt

		## input file
		[INPUT]: /home/projects/simon/ConnectionTool/testfiles/test.csv

		## Files which contains this paths will generate a warning message

		[WARNING]
		{
		/home/simon/
		}

		## Paths/files which won't be modified
		[ILLEGAL]
		{
		/home/POOL/
		}

		## Directory for backupfiles
		[FILE]: /home/projects/simon/ConnectionTool/testfiles/

		Die Datei wird standartmäßig von der ausführbaren Datei ict eingelesen.
		mit ./ict -f kann eine andere Datei als configfile angegeben werden.

	* doc

		Enthält zu einem späteren Zeitpunkt die Documentation des Codes.

	* flow

		flow/ enthält ein globales Makefile, welches die Makefiles in den anderen Verzeichnissen 
		importieren um Variablen zentral setzen zu können.

	* header

		Enthält die .h files des Ordners src/

	* lib

		lib sollte ursprünlich Dateien enthalten, die von anderen files benötigt werden.
		Im Moment ist lib ein unaufgeräumter Abstellplatz für header und cpp files, 
		die eigentlich in src/ und header/ gehören.
		Da noch irgendwie die boostlibrary eingebunden werden muss, wird diese vermutlich
		hier landen und die anderen files werden in src/ bzw. header/ wandern.

	* main

		Das Verzeichnis main enthält Das Hauptprogramm ict.cpp, welches mit dem Makefile, 
		das sich im selbigen Verzeichnis befindet gebaut wird. Andere Datein, die sich zur 
		Zeit noch dort befinden, sind überbleibsel aus Testzeiten und teils nicht mehr lauffähig.

	* obj

		enthält alle object datein welche aus den files in lib bzw. src erstellt werden.
		Diese werden zum Hauptprogramm in main/ gebunden.

	* testfiles

		Ordner für backups, wird beim auführen des Programms ict in bin erstellt.

	* vpi

		vpi enthält die Unterordner f9/ x1/ und src/

		* src

			Enthält Datein, die zu dem jeweiligen Project hinzugebunden werden, um
			die Hierarchy des Verilog-Projekts auszulesen.

		* f9, x1

			Enthält das Makefile, welches die Datein aus vpi/src/ zu dem Verilog-Projekt hinzufügt. 
			Eine Datei namens mod.txt, sollte nach dem kompilieren in den jeweiligen Verzeichnissen 
			existieren, welche die Hierarchy des jeweiligen Projekts enthält.

		* generate_modtxt.sh

			Ausführbares Shellscript, kann mit Parameter f9 oder x1 aufgeführt werden.
			Generiert die Textdatei mod.txt, welche die Hierarchie für das jeweilige Projekt 
			enthält.


# Erste Schritte

	* Das Projekt "bauen"

	Wird im Wurzelverzeichnis make build aufgerufen, so wird alles was nötig ist
	erstellt und kompiliert, bis auf die Hierarchie und die Inputdatei.
	Das Binary, welches zum ausführen des Programms aufgerufen wird, befindet sich im Verzeichnis
	bin/.
	Der Aufruf ./ict fügt dem Projekt neue Verbindungen, die in der Inputdatei spezifiziert sind hinzu.
	Der Aufruf ./ict -r stellt die Originaldatein wieder her.
	Dies sind bisher die einzig Sinnvollen aufrufe des Programms, zusätzliche Optionen folgen.

	* Hierarchie

	Um die Hierarchie der Projekte auslesen zu können wird das Shellscript
	generate_modtxt.sh mit parameter (f9 oder x1) aufgerufen.
	Zuvor muss allerdings der Pfad zu dem jeweiligen Projekt gesetzt werden.
	Dies geschieht in den ersten Zeilen des Scripts.

	Beispiel:
	
		X1_PATH="/home/projects/simon/x1/"
		F9_PATH="/home/projects/simon/f9/"

	Durch den aufruf von "./generate_modtxt.sh f9" wird die Datei mod.txt im selben
	Verzeichnis erzeugt, welche standartmäßig in der Datei paths.cfg eingebunden ist.
	
	Beispiel:

	[MODTXT]: /home/projects/simon/ConnectionTool/vpi/mod.txt

	Hier kann nur ein Pfad angegeben werden!

	* Konfiguration

	Die Datei paths.cfg in cfg/ wird zur Konfiguration genutzt.

	Erklärung zu den Labels:

	[INPUT]: Pfad zur input Datei. 

	Die Inputdatei enthält die zu verbindenden Instanzen.
	Formatiert werden diese wie folgt:
	pfad1 tab pfad2 tab input bitbreite name1|output bitbreite name2 tab

	Beispiel:

	f9_asic.f9_dft_i0.f9_design_i0	f9_asic.rfdb_r_FDB1	
	input [10:0] test_1_i_1|output [10:0] test_1_o_2|output [9:0] test_1_o_3	

	Das Zeichen | dient als trennzeichen, um mehrere inputs, bzw. ouputs hinzufügen zu können

	Die Pfade können direkt aus dem vcs kopiert werden, jedoch ist darauf zu achten, dass der Pfad
	auf f9_asic beinhaltet und nicht ....

	NOCH NICHT IMPLEMENTIERT!
	Optional kann das Programm unter bin/ mit dem flag -C gestartet werden, 
	welches dann im Hintergrund die Zwischenablage nach Pfaden durchsucht 
	und beim drücken von STRG + C den Pfad richtig manipuliert und der 
	Datei (welche noch irgendwo einzutragen ist, ka wo? ) hinzufügt.


	[WARNING]
	{
	Pfad1
	Pfad2
	.
	.
	.
	}

	und:

	[ILLEGAL]
	{
	Pfad1
	Pfad2
	.
	.
	.
	}


	Diese Labels enthalten die Pfade zu Verilog-datein, welche gesondert behandelt werden sollen.
	WARNING enthalten Pfade zu Verzeichnissen oder Datein, welche beim Verändern 
	eine Warnung ausgeben sollen.

	ILLEGAL enthalten Pfade zu Verzeichnissen oder Datein, welche nicht verändert werden dürfen.
	Das Programm wird abgebrochen. 
	( Noch einbauen, dass der Originalzustand des Projekts hergestellt wird)


	[FILE]:

	Enthät den Pfad zu den Backupfiles. Bevor eine Verbindung verändert wird, werden alle Datein, 
	die verändert werden in diesem Verzeichnis gesichert. 
	Mit dem Aufruf ./ict -r werden alle Datein in diesem Verzeichnis wieder an den 
	Ursprünglichen Platz kopiert und gelöscht. Hierbei wird der Urzuzstand hergestellt, 
	d.h. bevor eine Datei verändert wird, wird in diesem Verzeichnis geschaut, 
	ob die zu veränderte Datei schon existiert, wenn nicht wird diese dort hin kopiert, 
	anderenfalls wird die Datei ausgelassen.


# Probleme
	(NOCH ZU TUN)

