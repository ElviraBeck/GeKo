[![Build Status](https://travis-ci.org/GameEngineKoblenz/GeKo.svg)](https://travis-ci.org/GameEngineKoblenz/GeKo)

BITTE BEACHTEN:
- Unterstützt werden aktuell Windows (MinGW und MSVC) und Linux
- Internet-Verbindung muss während es ganzen Build-Vorgangs bestehen
- Es werden während des Vorgangs verschiedene benötigte Pakete aus dem Internet heruntergeladen (~300 MB)
- Der erste Build-Vorgang in der IDE kann daher (je nach Internetverbindung) einige Minuten in Anspruch nehmen
- Git muss installiert sein (Unter Windows reicht nicht "GitHub for Windows" oder ähnliches)

===============================================================================================================================	
> Anleitung unter Windows

Hinweis: Sollen mit der Dokumentation Graphen erstellt werden muss Graphviz vorher auf dem System installiert sein und die entsprechende Option in CMake ausgewählt werden. (Download:http://www.graphviz.org/Download..php)

- Öffne Cmake (min. Version ist 2.8)
- Oben bei "Where is the source code" auf "Browse Source" und den heruntergeladenen Ordner auswählen  (den Ordner in dem diese Datei liegt - Er muss die CmakeLists.txt Datei enthalten)
- Darunter bei "Where to build the binaries" den gleichen Pfad wählen und dahinter ein /BUILD setzen. Dadurch werden die Dateien in einen neuen Ordner erstellt.
- Danach unten auf "Configure" - Wenn es noch keinen BUILD Ordner in dem Verzeichnis gibt, fragt Cmake, ob er erstellt werden soll - mit "yes" bestätigen.
- Nun fragt Cmake nach dem generator. Hier ist es besonders wichtig, dass der richtige generator für deine IDE und den Compiler gewählt wird. Mit Finish bestätigen und Warten bis unten in der Konsole "Configuring done" erscheint.
- Danach über der Konsole auf "Generate klicken" und warten bis unten in der Konsole "Generating Done" erscheint (Tauchen dabei Warnings auf, können diese meist ignoriert werden)
- Im erstellten BUILD Ordner gibt es jetzt die entsprechenden Projektdateien.
- Diese können jetzt innerhalb der IDE gebuildet und verwendet werden. 
Achtung: Je nach Internetverbindung kann der build einige Minuten dauern, da die benötigten Pakete heruntergeladen werden!
	
	
Die Resources (Texturen, Sounds) müssen extra heruntergeladen werden. - wird noch geklärt)

Wichtig: Da wir die einzelnen BUILD Ordner auf keinen Fall ins GIT einchecken sollten, liegt im Hauptordner des Repositories
eine .gitignore Datei. Diese sorgt dafür, dass der Ordner ./sourceCode/BUILD nicht ins GIT geladen wird. Falls es weitere
Ordner gibt, die nicht mit dem GIT synchronisiert werden sollen müssen die entsprechenden Pfade in der Datei ergänzt werden

===============================================================================================================================	
> Anleitung unter Linux

Voraussetzung - zum compilen von GLFW werden unter Linux einige Standardpakete benötigt.
Falls nicht vorhanden, öffne das terminal und gebe folgende Befehle ein:


$ sudo apt-get install xorg-dev

$ sudo apt-get install libglu1-mesa-dev


Zwischendurch muss der sudo Befehl einmal mit dem Passwort bestätigt werden.
Die benötigten Pakete wurden heruntergeladen und installiert.

Weiterhin muss Assimp für Linux per Hand installiert und gebaut werden:


$ git clone git://github.com/assimp/assimp.git assimp 

$ sudo apt-get install libboost-dev 

$ sudo apt-get install zlib1g-dev 

$ cd assimp 

$ cmake -G 'Unix Makefiles' 

$ make 

$ sudo make install 

$ sudo ldconfig


Hinweis: Sollen für die Dokumentation Graphen erstellt werden muss Graphviz vorher auf dem System installiert sein und die entsprechende Option in CMake ausgewählt werden.
($ sudo apt-get graphviz)

- Öffne Cmake (min. Version ist 2.8)
- Oben bei "Where is the source code" auf "Browse Source" und den heruntergeladenen Ordner auswählen  (den Ordner in dem diese Datei liegt - Er muss die CmakeLists.txt Datei enthalten)
- Darunter bei "Where to build the binaries" den gleichen Pfad wählen und dahinter ein /BUILD setzen. Dadurch werden die Dateien in einen neuen Ordner erstellt.
- Danach unten auf "Configure" - Wenn es noch keinen BUILD Ordner in dem Verzeichnis gibt, fragt Cmake, ob er erstellt werden soll - mit "yes" bestätigen.
- Nun fragt Cmake nach dem generator. Hier ist es besonders wichtig, dass der richtige generator für deine IDE und den Compiler gewählt wird. Mit Finish bestätigen und Warten bis unten in der Konsole "Configuring done" erscheint.
- Danach über der Konsole auf "Generate klicken" und warten bis unten in der Konsole "Generating Done" erscheint (Tauchen dabei Warnings auf, können diese meist ignoriert werden)
- Im erstellten BUILD Ordner gibt es jetzt die entsprechenden Projektdateien.
- Diese können jetzt innerhalb der IDE gebuildet und verwendet werden. 
Achtung: Je nach Internetverbindung kann der build einige Minuten dauern, da die benötigten Pakete heruntergeladen werden!
	
Die Resources (Texturen, Sounds) müssen extra heruntergeladen werden. - wird noch geklärt)

Wichtig: Da wir die einzelnen BUILD Ordner auf keinen Fall ins GIT einchecken sollten, liegt im Hauptordner des Repositories
eine .gitignore Datei. Diese sorgt dafür, dass der Ordner ./sourceCode/BUILD nicht ins GIT geladen wird. Falls es weitere
Ordner gibt, die nicht mit dem GIT synchronisiert werden sollen müssen die entsprechenden Pfade in der Datei ergänzt werden

===============================================================================================================================	

Anweisungen zur Arbeit mit der Engine:

> Einfügen einer neuen Klasse in die Game-Engine
Neue Klassen müssen in den Ordner .\src\libraries\*GeKo-UNTERORDNER* eingefügt werden. Dabei sollte darauf geachtet werden,
dass die neue Klasse vom Zweck her in den passenden Unterordner einsortiert wird. Gibt es keinen passenden, kann ein neuer erstellt werden (siehe unten)

> Erstellen eines neuen Unterordners der GeKo-Engine
Der neue Ordner muss im Pfad .\src\libraries\ erstellt werden. Er muss namentlich mit "GeKo_" beginnen. Hinter dem Unterstrich muss mit großem Buchstaben
der Name des neuen Unterordners eingesetzt werden. (vgl. bestehende Ordner). Es ist ein englischer Name zu wählen. In den neuen Ordner muss eine CMakeLists.txt Datei aus
einem der bestehenden Ordner kopiert werden. (z.B. aus GeKo_Graphics)

> Anlegen einer neuen Executable (z.B. ein neues Example)
Um ein neues Executable anzulegen, muss in  .\src\executables ein neuer Ordner erstellt werden in den die entsprechenden .h und .cpp Dateien gehören. Zusätzlich muss eine CMakeLists.txt Datei vorhanden sein. Diese kann einfach aus einem der anderen Executable Ordnder kopiert werden.

> Hinzufügen von Resources (Texturen, Sounds...)
Resources gehören in das Verzeichnis .\resources\

> Erstellen von Diagrammen für die Dokumentation mit Graphviz
Wähle in Cmake die Option USER_GRAPHVIZ. Dafür muss Graphviz heruntergeladen und installiert sein.
Unter Windows muss der bin Ordner von Graphviz in die PATH Variable eingefügt werden.
(Download unter: http://www.graphviz.org/Download..php)
	
