<?php
/**
 * @file : index.php
 * Ce fichier est le 1er a être appellé 
 * Ce script fait partie de l'application Ls_Files_For_Alf
 * Dernière modification : $Date: 2016-12-22 07:00 $
 * @author    Laurent ARCHAMBAULT <archi.laurent@gmail.com>
 * @copyright Copyright 2016-2017 
 * @link      http://www.gnu.org/licenses/licenses.html
 * @package   root
 * @version   $Id: function.php,v 1.0 2016-12-22 07:00 $
 * @filesource
 *
 * This file is part of Users_files_for_Alf
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Users_Files_For_Alf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Users_Files_for_Alf; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
/* ============================================================== */
// evite la prise en compte en UTC si rien
date_default_timezone_set("Europe/Paris");
/* -------------------------------------------------------------------- */

error_reporting(0);	  			# 0 = rien ! ou E_ALL
ini_set('display_errors', FALSE);		# prod = false
ini_set('display_startup_errors', FALSE);	# prod = false

include_once("includes/configuration.inc");	# les paramètres
include_once("includes/functions.inc");		# les fonctions
include_once("includes/header.inc");			# l'entete HTML

echo "<title>USERS ALF</title>";	# Affiche ceci dans l'onglet du navigateur

#============================= 1ere PARTIE Authentification ===========================================
# Si rien en POST c'est que c'est le début donc on envois de quoi se loguer

        if ( (!isset($_POST['l'])) AND (!isset($_POST['p'])) ) {

        # on appelle la fonction pour le formulaire avec ce même fichier-$tab_link Cf includes/configuration.php
        form_auth($tab_link[0], $tab_link);
	}

 
#============================= 2eme PARTIE la suite + finale ===========================================
# seul "etat=1" est envoyé en GET, le reste est en POST
# etat = 1 permet de savoir que nous avons quitter le formulaire précédent

        if ( @$_GET['etat'] == '1')  {

# recupération du login / mot de passe
if (isset($_POST['l'])) { $login = $_POST['l'] ;   } else { $login = null ;}
if (isset($_POST['p'])) { $password = $_POST['p'] ;} else { $password = null;}

	# Affichage des informations sur la plateforme Alfresco
	get_infos_for_alfresco($url_for_alf,$login,$password);	# renvois le tableau $infos_alf[de 0 à 2]

        # affichage d'un message si les droits d'accès sont insuffisants, mauvais login...etc
        if (isset($infos_alf['error'])) { echo "<p class='p_error'>".$infos_alf['error']."</p>";exit(0); }

	echo "<div class='class_for_alf'>";
	echo "<p class='title_for_alf'>Votre plateforme : </p><h3>" .$infos_alf[0] ."</h3>";
	echo "<p class='title_for_alf'>Sa version : </p><h3>" .$infos_alf[1] ."</h3>";
	echo "<p class='title_for_alf'>Son dépôt racine : </p><h3>" .$infos_alf[2] ."</h3>";
	echo "</div>";


	# a partir d'ici c'est le traitement des données site par site
	# l'affichage se fait par 2 boucles imbriquées site puis users
	donne_sites($url_for_sites,$login,$password);
	
	$nbr_sites = count($infos_sites) ;	# je sais on recompte...les sites au total

	# en boucle en fonction du nbr de sites	
	for ($i=0;$i<$nbr_sites;$i++) {
		echo "<div class='div_site'>";

		echo "<span>\nSite : <p class='for_lign_site'>" .$infos_sites[$i]['title'] ."</p>"
		 ." <p class='for_lign_site_type'>( ".$infos_sites[$i]['visibility'] ."</p>"
		 ." ) - " .$infos_sites[$i]['guid'] ."</p>\n";

		echo " - <p class='for_lign_site_type'>" .$infos_sites[$i]['desc'];
		echo "</p></span>\n</div>\n";

	# pour chaque site on demande les utilisateurs
	donne_users($url_for_sites,$login,$password,$infos_sites[$i]['id']);
	
        	$nbr_users = count($infos_users);	# on compte les utilisateurs

		# et en fonction des users on affiche le tout
	        for($t=0; $t<$nbr_users;$t++) {
			
			echo "<div class='div_users'>";
	
			echo "<div class='div_users_lign1'>\n"
			."<span class='for_user_login'>Utilisateur : \n"
			."<p class='for_login'>" .$infos_users[$t]['login'] ."</p>\n";

			# $tab_role est defini dans configuration, de la on prend le role d'origine
			# et on modifie le texte puis le code CSS...tout simplement
			echo "<p class='".$tab_role_css[$infos_users[$t]['role']];
			echo "'>".$tab_role[$infos_users[$t]['role']]  ."</p>\n";
			

			# diverses modifications d'affichage si ACTIF ou pas si Notification ou pas
			if ($infos_users[$t]['actif'] == TRUE ) {
			echo "<p class='for_users_actif'>Activé</p>\n";
			echo "\n";
			} else {
			echo "<p class='for_users_actif barre'>Activé</p>\n";
			echo "\n";
			}
			
			if ($infos_users[$t]['notification'] == TRUE ) {
			echo "<p class='for_users_notif'>Notification OK</p>\n";
			echo "</span></div>\n</div>\n";
			} else {
			echo "<p class='for_users_notif red'>Notification NOK</p>\n";
			echo "</span></div>\n</div>\n";
			}
		}
	}

}	# fin si etat = 1	
	

include("includes/footer.inc");			# la fin du HTML
?>
