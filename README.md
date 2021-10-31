# SmartCars

SmartCars

Modification to make after cloning the project:

Your sdk version in VS 2019 : go to project properties > Configuration Properties > In general properties > Windows SDK version Your QT version :

go to extensions > QT VS tools > options > versions > get the version name go in project properties > Configuration properties > Qt Project Settings > paste version name in QT installation

go in project properties > Configuration properties > VC++ directories > under general > external include directories > add your include of QT installation example : YourDISK:\QT\QTversion\version\msvc2017_64\include

## Librairie Osmium
Pour mettre en place la librairie osmium il faut placer le répertoire include dans le dossier du projet et ensuite rajouter ce dossier au niveau de project properties > Configuration properties > VC++ directories > under general > external include directories > ";include"

### bzip2
http://gnuwin32.sourceforge.net/packages/bzip2.htm
https://netcologne.dl.sourceforge.net/project/gnuwin32/bzip2/1.0.5/bzip2-1.0.5-setup.exe

### zlib
http://gnuwin32.sourceforge.net/packages/zlib.htm
https://altushost-swe.dl.sourceforge.net/project/gnuwin32/zlib/1.2.3/zlib-1.2.3.exe

### Pour inclure les dépendances installés dans le projet
C:\Program Files (x86)\GnuWin32\include
