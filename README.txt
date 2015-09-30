Comment un shell fonctionne ?

Il y a trois étapes majeures:

1) La gestion de l'entrée: Gestion des touches, des raccourcis, de l'édition de la ligne, des sous shells etc...
	-Dans le SH1 cela se résume à un get_next_line.
	-Dans le SH3 cela revient à gérer les termcaps et préprocess les inhibiteurs pour vérifier la terminaison
	de la ligne.

2) L'interprétation de ligne afin de l'éxécuter, par exemple la gestion des inhibiteurs (', ", \),
la gestion des backquotes '`' afin d'éxécuter une commande avant l'exécution de la commande totale,
la préparation des redirections (|, <, <<, >, >>), la préparation des conditions (&&, ||) ainsi que
des commandes multiples à l'aide de ';' etc...
	-Dans SH1 cela se résume à une recherche du path du binaire dans dans le dossier courant puis
	en cas d'échec une recherche dans le PATH (La variable d'environnement).
	-Dans le SH2 cela revient à gérer les redirections à l'aide de dup2, ainsi que les commandes multiples.

3) L'exécution de la ligne interprétée.
	-Dans SH1 cela se résume à une succession d'appels à fonction (builtins: "cd", "setenv", "unsetenv", "env", "exit") ou d'execs.
	-Dans SH2 cela revient à gérer les sorties des binaires exécutés en fonction des redirections détectées.




Procédons donc en plusieurs sous-étapes:
			-Partie I) Récupération de l'environnement et builtin "env":
	Premièrement, qu'est-ce que l'environnement ? C'est un tableau de chaines charactères (char **) permettant
	d'indiquer différentes informations liées au fonctionnement même du shell, un peu comme une version clean
	des structures "env" que nous utilisons. Comment le récupérer ? Rien de plus simple, le main le peut.
	En effet, l'environnement se trouve juste après l'int "argc" et le char ** "argv".
	Ainsi, pour le récupérer il faut déclarer le main comme suit: "int main(int argc, char **argv, char **env)"
	Pour vérifier, on créé le builtin builtin_env qui affiche la variable d'environnement.
	Pour rendre le fonctionnement générique on introduit notre t_env constitué pour l'instant uniquement d'un char **,
	environment_table. t_env sera ainsi défini comme suit:
	
	typedef struct	s_env {
	char			**env_table;
	}				t_env;

	Ainsi, si tout fonctionne, l'appel de "builtin_env(env)" devrait afficher la même chose que "env" dans n'importe quel shell.


			-Partie II) 