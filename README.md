# User Perspective AR
Ce projet a été réalisé dans le cadre de l'option Réalité Virtuelle par Maxence Fort, Benjamin Heinrich, Chloé Leric et Paul Bertrand

## Installation

### Sur Linux et Mac
* Cloner le repository: ``` git clone https://github.com/LarbiKems/P2RV_Sujet1.git```
* Se rendre dans le dossier ***P2RV_Sujet1/***
* Lancer la commande ``` cmake .```
* Lancer la commande ``` make```
* L'exécutable est créé dans le dossier *bin/* et porte le nom ***P2RV_Sujet1***

### Sur Windows (avec Visual Studio 2017)
* Cloner dans un dossier quelconque le repository (en utilisant [git bash](https://gitforwindows.org/) par exemple)
    ``` 
    git clone https://github.com/LarbiKems/P2RV_Sujet1.git
    ```
* Créer un projet vide en *C++* dans Visual Studio
* Ajouter tous les fichiers du répertoire *src/* du repository au projet:
    * clic droit sur le projet
    * Ajouter > Élément existant 
    * Sélectionner tous les fichiers du dossier *src/* 

* Configurer le projet pour utiliser OpenCV en ajoutant les feuilles de propriétés adéquates ou en suivant [ce tutoriel](https://hippocampus.ec-nantes.fr/mod/resource/view.php?id=12799)
* Copier le répertoire *resources/* du repository à la racine du projet Visual Studio (à l'endroit où se trouve le fichier *.vcxproj* correspondant au projet)
* Cliquer sur *Débogueur Windows Local* dans Visual Studio

## Versions

### Version_Thales
La seule version totalement fonctionnelle est la version nommée *Version_Thales*. Pour y accéder, il faut se positionner sur la branche *Version_Thales*.
#### Fonctionnement

* Lancer l'application
* Dans le terminal, indiquez le nombre de caméra dont vous disposez. Si vous ne disposez que d’une caméra, celle-ci sera utilisée en tant que caméra de face.
* Dans le terminal, entrez la largeur entre vos 2 yeux (en cm) puis appuyez sur *Entrer*
* Cliquez sur la fenêtre *Calibration de la caméra frontale* et suivez les instructions inscrites dans le terminal afin de calibrer la caméra avant: placez vous à 50 cm de l’écran, et appuyez sur *c*, puis sur *Entrer* lorsque le terminal vous l’indique.
* À présent, dans la fenêtre *Résultat* vous devriez voir apparaître l’image de la caméra principale (ou de Lenna si vous n’avez qu’une caméra de branchée) qui évolue en fonction de votre position. Vous pouvez contrôler votre position en affichant la fenêtre Image après détection du visage.


