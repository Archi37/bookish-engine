<?php
/**
 * @file : rep-file.php
 * Ce fichier doit lister le contenue des fichiers d'un site précis 
 * Ce script fait partie de l'application Users_Files_for_Alf
 * Dernière modification : $Date: 2016-12-22 07:00 $
 * @author    Laurent ARCHAMBAULT <archi.laurent@gmail.com>
 * @copyright Copyright 2014-2015 
 * @link      http://www.gnu.org/licenses/licenses.html
 * @package   root
 * @version   $Id: function.php,v 1.0 2016-12-22 07:00 $
 * @filesource
 *
 * This file is part of Users_Files_for_Alf
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Users_Files_for_Alf is distributed in the hope that it will be useful,
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

error_reporting(0);  			# 0 = rien(prod) ou E_ALL
ini_set('display_errors', FALSE);		# prod = false
ini_set('display_startup_errors', FALSE);	# prod = false

include_once("includes/configuration.inc");	# les paramètres
include_once("includes/functions.inc");		# les fonctions
include_once("includes/header.inc");		# l'entete HTML

echo "<title>REP-FILES ALF</title>";	# Affiche ceci dans l'onglet du navigateur

?>
<div id="container"> 


<?php
#============================= 1ere PARTIE Authentification ===============================================================
# donc si aucun POST c'est que rien n'est fait alors on affiche de quoi s'authentifier

	if ( (!isset($_POST['l'])) AND (!isset($_POST['p'])) ) {

	# on appelle la fonction pour le formaulaire avec ce même fichier
	form_auth($tab_link[1],$tab_link);

} # Fin test si L & P vides !


#============================= 2eme PARTIE Selection SITE + options=========================================================
# une fois authentifié il faut sélectionner le site et éventuellement les options
# ?etat=1 est là pour çà, c'est le seul envoyé en GET

	if ( @$_GET['etat'] == '1')  {

$sous_rep = null;
# recupération du login / mot de passe
if (isset($_POST['l'])) { $login = $_POST['l'] ;   } else { $login = null ;}
if (isset($_POST['p'])) { $password = $_POST['p'] ;} else { $password = null;}


# Préparation à l'affichage du bandeau haut
get_infos_for_alfresco($url_for_alf,$login,$password);	# renvois le tableau $infos_alf[de 0 à 2]
	# affichage d'un message si les droits d'accès sont insuffisants
	if (isset($infos_alf['error'])) {
		echo "<p class='p_error'>".$infos_alf['error']."</p>";
		# echo "<p class='p_error'(>".$curl_errno($ch).")</p>";
		exit(0);
	 }	
	
	# Affichage du bandeau en haut
	echo "<div class='class_for_alf'>";
	echo "<p class='title_for_alf'>Votre plateforme : </p><h3>" .$infos_alf[0] ."</h3>";
	echo "<p class='title_for_alf'>Sa version : </p><h3>" .$infos_alf[1] ."</h3>";
	echo "<p class='title_for_alf'>Son dépôt racine : </p><h3>" .$infos_alf[2] ."</h3>";
	echo "</div>";
?>
	<!-- Le formulaire pour l\'étape finale vers etat=2-->
        <form class='myformlogin' method='post' action='rep-file.php?etat=2'>

	<div class='class_for_alf'>
	<p class='select_site'>
		
		<?php # Affichage du SELECT qui contient les sites (en fonction des autorisations) ?>
		<b>Site :</b>
<?php		donne_select_sites($url_for_sites,$login,$password);	# forme le SELECT avec les sites si AUTH Ok !
?>
	
	</p>
	<p class='check_date'><input type='checkbox' value='1' name='chk_date' tabindex='2' checked >Afficher dates</p>
	<p class='check_stor'><input type='checkbox' value='1' name='chk_stor' tabindex='3' checked >Afficher le storage</p>
	<p class='check_mim'><input type='checkbox' value='1' name='chk_mime' tabindex='4' checked >Afficher MIME</p>
	
<?php	# on re injecte le login+mot de passe en hidden pour les récuperer après

	echo "<input type='texte' name='l' hidden value='{$login}'/>";
	echo "<input type='texte' name='p' hidden value='{$password}'/>";
?>	
	<button class="btn_flat_vert add_btn" type="submit">Envoyer &raquo</button>&nbsp;<br>&nbsp;
	</div>
	</form>

<?php
	# Fin test si login+mdp present

}
#--- Fin boucle site 

#============================= 3eme PARTIE Affichage =====================================================================
# traitement proprement dit du site concerne pour trouver les répertoires et les fichiers
# ?etat=2 represente cette étape

	if ( @$_GET['etat'] == '2')  {

# recupere des indispensables
if (isset($_POST['l'])) { $login = $_POST['l'] ;   } else { $login = null ;}
if (isset($_POST['p'])) { $password = $_POST['p'] ;} else { $password = null;}
if (isset($_POST['sel_site'])) { $site = $_POST['sel_site']; }

global $options;

# prepare les options pour l'affichage selectif :
if (isset($_POST['chk_date'] )) { $options['date'] = 1; } else { $options['date'] = null; }
if (isset($_POST['chk_stor'] )) { $options['stor'] = 1; } else { $options['stor'] = null; }
if (isset($_POST['chk_mime'] )) { $options['mime'] = 1; } else { $options['mime'] = null; }

# on envois l'affichage du bandeau haut
get_infos_for_alfresco($url_for_alf,$login,$password);	# renvois le tableau $infos_alf[de 0 à 2]
	
	if (isset($infos_alf['error'])) {
		echo "<p class='p_error'>".$infos_alf['error']."</p>";
		#echo "<p class='p_error'(>".$curl_errno($ch).")</p>";
		exit(0);
	 }	

	# Affichage du bandeau Haut
	echo "<div class='class_for_alf'>";
	echo "<p class='title_for_alf'>Votre plateforme : </p><h3>" .$infos_alf[0] ."</h3>";
	echo "<p class='title_for_alf'>Sa version : </p><h3>" .$infos_alf[1] ."</h3>";
	echo "<p class='title_for_alf'>Son dépôt racine : </p><h3>" .$infos_alf[2] ."</h3>";
	echo "<p class='title_for_alf'>Site : </p><h3>" .$site ."</h3>";
	echo "</div>";

	# et voilà ca requete pour avoir les REP/Fichiers
	donne_file_and_rep($url_for_files,$login,$password,$site,$options);

        @$nbr_files = intval(count($obj['objects'])) ;	# on compte nbr fichiers / boucle après
	
# var_dump($_POST);echo "<br>"; var_dump($obj);

	# tant que des fichiers/répertoires sont présents çà tourne !
	for ($t=0;$t<$nbr_files;$t++) {
		# ici on recherche les répertoires, ensuite on affiche ce meme répertoire
		if ($obj['objects'][$t]['object']['properties']['cmis:objectTypeId']['value'] == "cmis:folder" ) {
               	
			# il faut que les espaces soient encodés en %20 sinon echec !
			$tmp_site = str_replace(" ","%20",$obj['objects'][$t]['object']['properties']['cmis:path']['value']);
               		
			if_folder($url_for_rep,$login,$password,$tmp_site,$options);
               	}

               	if (@$obj['objects'][$t]['object']['properties']['cmis:contentStreamId']['value'] !== NULL) {
		affiche_document($obj,$t,$options);
		}	
	}		
}
#--- Fin boucle si POST 

include("includes/footer.inc");			# la fin du HTML
?>

</div> <!-- Fin container -->

