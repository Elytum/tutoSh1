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
	Pour des raisons d'implémentations de futurs builtins, il est plus pratique d'avoir une copie du tableau de variables
	d'environnement au lieu d'une simple copie du pointeur, il est donc préférable de faire un strdup sur chaque sous_élément de env.




			-Partie II) Gestion de la récupération de la ligne:

	Maintenant qu'on a pu récupérer notre environnement on veut récupérer notre ligne pour pouvoir par la suite exécuter notre
	builtin env. On appel get_next_line et pour vérifier on lance get_next_line puis on écris la ligne.
	Une fois ceci fait, on peut observer un problème de distinction entre la ligne entrée et la ligne de sortie,
	d'où l'intérêt du préfixe "?> " devant le prompt, on modifie donc l'appel à get_next_line de sorte à ce que le programme
	puisse par exemple produire ceci:
	
		Début du programme:
	?> Bonjour
	Bonjour
	?> La prochaine ligne sera vide !
	La prochaine ligne sera vide !
	?> 

	?>
		Fin du programme:




			-Partie III) Récupération de la commande à exécuter:

	Maintenant, nous pouvons lancer le builtin env, et nous pouvons récupérer une ligne, mais comment savoir si la ligne demande
	de lancer le builtin env, ou nous donne la recette des crèpes au whisky.
	Pour se faire, on ajoute la fonction: char *get_called_command(char *line) de sorte à ce que:
	Si line = "env", get_called_command retourne "env",
	si line = "env lolilol", get_called_command retourne "env",
	si line = "crepes_whisky", get_called_command retourne "crepes_whisky",
	si line = "tiramisu c'est meilleur que ile_flotante !", get_called_command retourne "ile_flotante".
		En effet, c'est une exception, car la fonction se doit d'être intelligente et de reconnaître les erreurs ^^.
	Blague à part, voici ce que doit faire la fonction.




			-Partie IV) Et enfin, jouons aux LEGO !

	Nous pouvons actuellement: Demander une ligne, connaître la fonction que l'utilisateur demande et lancer env, tentons d'assembler tout ça.
	Un peu d'imaginations, essaye, en cas de problème, lis le bloc suivant.
					On lance en boucle le promp, si line n'est pas vide, on lance get_called_command pour savoir la ligne que l'utilisateur
					veut lancer. On la compare au nom de notre builtin, env. S'il n'y a aucune différence et que donc la commande demandée est
					env, on lance builtin_env, sinon on indique que la commande n'a pas été trouvée.
	Et voilà, un shell qui commence à bien fonctionner !




			-Partie V) Maintenant qu'on a un shell en LEGO, il nous faut plus de blocs, pour l'instant, on a un carré,
							un vaisseau c'est carrément plus classe !

	Commençons par le builtin exit. Il fonctionne comme suit:
		-Si on indique une valeur correcte (0, 42, -1 ...) il lance un exit sur cette valeur.
		-Si on indique rien, il lance un exit sur 0.
		-Sinon, en cas de paramètre invalide, il lance aussi exit sur 0.
		Tentons donc de créer la fonction builtin_exit comme suit:
			-Si line = "exit", lance exit (0).
			-Si line = "exit 0", lance exit (0).
			-Si line = "exit -1", lance exit (-1).
			-Si line = "exit +1", lance exit (1).
			-Si line = "exit lol", lance exit (0).
			Petits détails, nécessaires à ces tests:
				-Pour obtenir la valeur avec laquelle le shell est sortis, on peut lancer dans le shell qui a permis de le lancer la commande "echo $?"
					qui affichera la dernière valeur du shell en cours, ce qui correspondrait dans le cas présent à la valeur de retour de exit.
				-On observera que "exit -1" aura pour valeur "255" et non "-1", c'est du au fait que la valeur soit codée sur un unsigned char,
					qui ne peut être négatif et va donc overflow, d'où -1 -> 255.

	Continuons avec le builtin setenv. Il n'est pas explicitement définis, et n'existe pas partout,
		la définition sur "http://nature.berkeley.edu/~casterln/tcsh/Builtin_commands.html" est:
			"Without arguments, prints the names and values of all environment variables. Given name,
			sets the environment variable name to value or, without value, to the null string.".
		Donc:
			-Si on indique aucun argument, il lance env.
			-Si on indique un argument, il ajoutera/changera la variable donnée en argument et lui donnera la valeur "".
			-Si on indique deux arguments, il ajoutera/changera la variable donnée en premier argument et lui donnera la valeur du deuxième argument.
		Ainsi:
			"setenv" lancera un "env"
			"setenv lol" ajoutera/changera "lol" dans le tableau de variables d'environnement et lui donnera la valeur "" -> "lol=" dans env.
			"setenv couleur bleu" ajoutera/changera "couleur" dans le tableau de variables d'environnement et lui donnera la valeur "bleu" -> "couleur=bleu" dans env.
