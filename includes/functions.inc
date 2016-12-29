<?php
/*
 * Ce fichier rassemble les X functions nécessaires
 * 
 * Dernière modification : $Date: 2016-12-22 07:00 $
 * @author    Laurent ARCHAMBAULT <archi.laurent@gmail.com>
 * @copyright Copyright 2016-2017 
 * @link      http://www.gnu.org/licenses/licenses.html
 * @package   root
 * @version   $Id: function.inc,v 1.0 2016-12-22 07:00 $i
 * @filesource
 *
 * This file is part of users_files_for_alf
 *
 * Users_files_for_alf is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Users_files_for_alf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Users_files_for_Alf; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
/* ============================================================== */
/* ============================================================== */
# Cette fonctions renvois des informations sur la version d Alfresco
# Elle retourne un tableau qui contien trois données
# L'URL 'sites" fonctionne bien pour çà...
#
/* ============================================================== */
# Cette fonction en fait que présenter de quoi s'authentifier
# $page doit contenir le fichier destination complet (!)
# Affiche une entete puis un formulaire pour se loguer

function form_auth($page,$tab_link) { 


if ($page == $tab_link[0]) { 

	echo "<div id='box_haute'><div class='for_link'><a href='{$tab_link[1]}'>Liste les fichiers&nbsp;&nbsp;&nbsp;</a></div></div>";

} else { 
	echo "<div id='box_haute'><div class='for_link'><a href='{$tab_link[0]}'>Liste les utilisateurs&nbsp;&nbsp;&nbsp;</a></div></div>";
}

echo "<form class='myformlogin' method='post' action='{$page}?etat=1'>";
?>
<div class="for_login">
        <span class="form_login"><p class"for_decal">Utilisateur :&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</p><span class="red">*</span>
        <input class="decal_left" type="text" required="required" name="l" size="50" minlength="2" maxlength="150" tabindex="1" autofocus/>
        </span>
        <br>
        <span class="form_mdp"><p class="for_decal">Mot de passe :</p><span class="red">*</span>
        <input class="decal_left" type="password" required="required" name="p" size="50" maxlength="24" tabindex="2" pattern="[a-zA-Z-0-9\ _-@&]{3,}" />
</span>
<p class="btn_flat_vert"><button class="btn_flat_vert" type="submit" tabindex="3">Envoyer &raquo</button></p>&nbsp;

</div>
</form>

<?php
}

/* ============================================================== */
# Cette fonctions renvois des informations sur la version d Alfresco
# Elle retourne un tableau qui contien trois données
# L'URL 'sites" fonctionne bien pour çà...
#
function get_infos_for_alfresco($url,$login,$password) {
global $infos_alf;
$result = null;
$obj = null;
$errNo = null;
	$ch = curl_init(); 				# on initialise php-curl

	curl_setopt($ch, CURLOPT_URL,$url);			
	curl_setopt($ch, CURLOPT_RETURNTRANSFER,1);		
	curl_setopt($ch, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);	
	curl_setopt($ch, CURLOPT_USERPWD, "$login:$password");	

	$result=curl_exec($ch);				# if result === false = probleme

	# if(curl_errno($ch)) { $errNo = curl_errno($ch);}	# si erreur la memoriser
 
	curl_close($ch);				# on referme la liaison

	$obj = (json_decode($result,true));	# légère mise en forme

#var_dump($obj); # DEBUG

	# si $obj est vide c'est que le lien/serveur ne sont pas bons
	if (@$obj == NULL) { $infos_alf['error'] = "<h2 class='msg_error'>Aucune donnée en retour, vérifiez vos données !</h2>"; return; }	
	if (@$obj['status']['code'] == 401) { $infos_alf['error'] = "<h2 class='msg_error'>Vous n'avez pas les droits nécessaires !</h2>"; return; }	

#var_dump($obj);

	$infos_alf[] = $obj['-default-']['productName'];	# Le type d'Alfresco
	$infos_alf[] = $obj['-default-']['productVersion'];	# La version
	$infos_alf[] = $obj['-default-']['rootFolderId'];	# le noeud de ref
    return $infos_alf;
}
/* ============================================================== */
function donne_sites($url,$login,$password){
#
# Cette fonction demande 3 options et retourne un tableau avec des données
# on commence par une requete PHP-CURL puis on récupere les infos qui vont bien
# dans $obj 

global $infos_sites;
$obj = null;
$result=null;
$infos_sites = null;

	$ch = curl_init();
	curl_setopt($ch, CURLOPT_URL,$url);
	curl_setopt($ch, CURLOPT_RETURNTRANSFER,1);
	curl_setopt($ch, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
	curl_setopt($ch, CURLOPT_USERPWD, "$login:$password");

	$result = curl_exec($ch);
	$obj = (json_decode($result,true));
	curl_close($ch);		# on referme la liaison

#var_dump($obj);	# DEBUG

	$nbr_sites = intval($obj['list']['pagination']['count']);	# il faut connaitre le nombre de sites

	for($i=0; $i<$nbr_sites;$i++) {			# la boucle qui permet de recuperer les données que l'on veut uniquement

		$infos_sites[$i]['title'] = $obj['list']['entries'][$i]['entry']['title'];
		$infos_sites[$i]['visibility'] = $obj['list']['entries'][$i]['entry']['visibility'];
		$infos_sites[$i]['id'] = $obj['list']['entries'][$i]['entry']['id'];
		$infos_sites[$i]['guid'] = $obj['list']['entries'][$i]['entry']['guid'];
		$infos_sites[$i]['desc'] = $obj['list']['entries'][$i]['entry']['description'];
	}

return $infos_sites ;	# Le tableau + infos est transmis complet
}
/* ============================================================== */
# On se base sur l'URL $url_for_sites ensuite on ajoute le site via son ID (nom court)
# et non son nom long, celui qui est vu sur ALF. Pour lister uniquement la librairie de docs
# il faut ajouter apres le site /documentLibrary
# autre exemple possible : dataLists/?cmis:document/descendant';
# autre options a essayer ?maxItems=200
 
function donne_file_and_rep($url,$login,$password,$site,$options){
global $obj;

$obj = null ;
$result = null;

	# Mise en forme de l'URL avec ou sans ajout en fonction de USERS ou FILES a rechercher
	if ($site == NULL) { $newUrl = $url; } else { $newUrl = $url .$site ."/documentLibrary"; }

	$ch = curl_init();					# initialisation
	curl_setopt($ch, CURLOPT_URL,$newUrl);			
	curl_setopt($ch, CURLOPT_RETURNTRANSFER,1);		
	curl_setopt($ch, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);	
	curl_setopt($ch, CURLOPT_USERPWD, "$login:$password");	# envois login+passwd oblifatoires !

	$result = curl_exec($ch);				# récupération des données
	curl_close($ch);					# on referme la liaison
	$obj = (json_decode($result,true));			# Mise en forme des données

#var_dump($obj);

return $obj;
}
/* ============================================================== */
# Cette fonction renvois les utilisateurs d'un site précis
# Le tout est retourné dans le tableau $infos_users
#
function donne_users($url,$login,$password,$site){
global $infos_users;
$obj = null ;
$return=null;
$infos_users= null;
$newUrl = $url .$site ."/members";

	$ch = curl_init();
	curl_setopt($ch, CURLOPT_URL,$newUrl);
	curl_setopt($ch, CURLOPT_RETURNTRANSFER,1);
	curl_setopt($ch, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
	curl_setopt($ch, CURLOPT_USERPWD, "$login:$password");

	$result = curl_exec($ch);
	curl_close($ch);		# on referme la liaison
	$obj = (json_decode($result,true));

# var_dump($obj);	# DEBUG

	$nbr_users = intval(count($obj['list']['entries']));

	for($i=0; $i<$nbr_users;$i++) {			# la boucle qui permet de recuperer les données que l'on veut uniquement

		$infos_users[$i]['role'] = $obj['list']['entries'][$i]['entry']['role'];
		$infos_users[$i]['login'] = $obj['list']['entries'][$i]['entry']['person']['id'];
		$infos_users[$i]['notification'] = $obj['list']['entries'][$i]['entry']['person']['emailNotificationsEnabled'];
		$infos_users[$i]['actif'] = $obj['list']['entries'][$i]['entry']['person']['enabled'];
	}
return $infos_users; 	# Le tableau + infos est transmis complet
}
/* ============================================================== */
# $options permet d'afficher ou pas certaines options
# cette fonction scrute la librairie puis fait le tri entre les documents et les
# répertoires, et affiche le contenu qui va bien
# 
function if_folder($url,$login,$password,$site,$options) {
$obj2 = null;

	$newUrl = $url .$site; # ."/documentLibrary"; 

	# if faut enlever un bout de l'URL complète pour faire plus joli
	$repert_aff = str_replace($url,"",$newUrl);
	# on enlve le mot /sites pour ne garder que le site en question au début
	$repert_aff = str_replace("/Sites","",$repert_aff);

	# on prepare et lance la requete CURL
	$ch = curl_init();
	curl_setopt($ch, CURLOPT_URL,$newUrl);
	curl_setopt($ch, CURLOPT_RETURNTRANSFER,1);
	curl_setopt($ch, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
	curl_setopt($ch, CURLOPT_USERPWD, "$login:$password");

	$result = curl_exec($ch);	# le résultat brute est ici
	curl_close($ch);		# on referme la liaison
	
	$obj2 = (json_decode($result,true));			# on encode au format json et non linear
        $nbr_files = intval(count($obj2['objects'])) ;		# on compte le nbr de fichier/répertoire à la racine
       
#var_dump($obj2); 
	
	# mise en forme des répertoires pour affichage, remplace %20 par 1 espace !
	echo "<div class='repert_for_doc'>Rép: <p>".str_replace("%20"," ",$repert_aff) ."</p></div>\n";	

	for ($t=0;$t<$nbr_files;$t++) {

               	# permet de savoir si c'est un document ou pas, puis affiche les documents :
		if (@$obj2['objects'][$t]['object']['properties']['cmis:contentStreamId']['value'] !== NULL) {
                       affiche_document($obj2,$t,$options);
		} 
		
		# permet de deceler un répertoire puis affiche le répertoire :
                if ($obj2['objects'][$t]['object']['properties']['cmis:objectTypeId']['value'] == "cmis:folder" ) {
                    if_folder($url,$login,$password,$obj2['objects'][$t]['object']['properties']['cmis:path']['value'],$options);
                }
	}
return;
}
/* ============================================================== */
# Cette fonction affiche les documents d'un répertoire avec ou sans les options

function affiche_document($obj,$t,$options) {
		
	# affichage du nom du fichier + taille + créé par ...
	echo "<div class='for_doc'><span><p class='name_for_doc'>\n";
	echo $obj['objects'][$t]['object']['properties']['cmis:name']['value'] ."</p>";
		$taille = $obj['objects'][$t]['object']['properties']['cmis:contentStreamLength']['value'] ;
	echo "<p class='taille_for_doc'>( " .round($taille/1024,2) ." Ko )</p>\n" ;
	
	# Créé par :
	echo "<p class='for_login'>[".$obj['objects'][$t]['object']['properties']['cmis:createdBy']['value'] ." ]</p>\n";
	
	# option affichage de la date ou pas
	if ($options['date']) {
		$date_creat = $obj['objects'][$t]['object']['properties']['cmis:creationDate']['value'] ."\n";
		echo "<p class='dates_for_doc'>[ ".date('d-m-Y', $date_creat/1000);
		$date_modif = $obj['objects'][$t]['object']['properties']['cmis:lastModificationDate']['value'] ."]\n";
		echo " #[" . date('d-m-Y', $date_modif/1000) ."]"; 
	    	echo "(". (date('m',time() - ($date_modif/1000))-1) ."m)</p>";
	}
	
	# option affichage du storage ou pas
	if ($options['stor']) { echo "<p class='store_for_doc'>" .$obj['objects'][$t]['object']['properties']['cmis:contentStreamId']['value'] ."</p>\n"; }
	
	# Affichage du type MIME ou pas
 	if ($options['mime']) { echo "<p class=''>".$obj['objects'][$t]['object']['properties']['cmis:contentStreamMimeType']['value'] ."</p>\n"; }
 
	echo "</div>\n";
}

/* ============================================================== */
function donne_select_sites($url,$login,$password){
#
# Cette fonction renvois un <SELECT> avec les noms des sites
# on commence par une requete PHP-CURL puis on récupere les infos
# dans $obj 

global $infos_sites;
$obj = null;
$result=null;
$infos_sites = null;

	$ch = curl_init();
	curl_setopt($ch, CURLOPT_URL,$url);
	curl_setopt($ch, CURLOPT_RETURNTRANSFER,1);
	curl_setopt($ch, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
	curl_setopt($ch, CURLOPT_USERPWD, "$login:$password");

	$result = curl_exec($ch);
	$obj = (json_decode($result,true));
	curl_close($ch);		# on referme la liaison

#var_dump($obj);

	@$nbr_sites = intval($obj['list']['pagination']['count']);	# il faut connaitre le nombre de sites

	# début du SELECT c'est sel_site qui est renvoye en POST avec 'id' du site (nom compressé/court)
	echo "<select class='select_sites' name='sel_site' required>\n";
	echo "<option name='sel_comm' value='' ></option>\n";

	for($i=0; $i<$nbr_sites; $i++) {			# la boucle qui permet de recuperer les données que l'on veut uniquement

	$infos_sites[$i]['title'] = $obj['list']['entries'][$i]['entry']['title'];
	$infos_sites[$i]['visibility'] = $obj['list']['entries'][$i]['entry']['visibility'];
	$infos_sites[$i]['guid'] = $obj['list']['entries'][$i]['entry']['guid'];
	$infos_sites[$i]['id'] = $obj['list']['entries'][$i]['entry']['id'];

  
        echo "<option name='sel_site' value='{$infos_sites[$i]['id']}' >";
        echo $infos_sites[$i]['title'] ." - " .$infos_sites[$i]['visibility'] ;
        echo "</option>\n";
  	}
echo "</select>";	# Fin du SELECT
}
/* ============================================================== */

/* ============================================================== */
?>
