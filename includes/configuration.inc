<?php
#
# quelques variables à définir 
#

# le tableau des 2 pages concernées dans l'ordre pour faire les liens en
# du formulaire d'authentification !

$tab_link[0] = "index.php";
$tab_link[1] = "rep-file.php";


# Exporte des infos sur Alfresco uniquement (entête haute une fois logué)
$url_for_alf = 'http://127.0.0.1:8080/alfresco/api/-default-/public/cmis/versions/1.1/browser/';

# URL qui sera utilisé par défault pour lister les docs...etc
$url_for_sites = 'http://127.0.0.1:8080/alfresco/api/-default-/public/alfresco/versions/1/sites/';
# liste les users - Meme URL que ci-dessus mais :
# sera ajouté automatiquement le site + "/members" à la fin

# Liste les documents type contacts, tache...non utilisé pour le moment
#$url='http://127.0.0.1:8080/alfresco/api/-default-/public/cmis/versions/1.1/browser/root/sites/SIC/dataLists/?cmis:document/descendant';

# comme le nom leur indique :
$url_for_files='http://127.0.0.1:8080/alfresco/api/-default-/public/cmis/versions/1.1/browser/root/sites/';
$url_for_rep  ='http://127.0.0.1:8080/alfresco/api/-default-/public/cmis/versions/1.1/browser/root';

# liste les dashlets d'un site ...pour infos 
# $url='http://127.0.0.1:8080/alfresco/api/-default-/public/alfresco/versions/1/sites/SIC/containers';

# pour la conversion des rôles en Français :
$tab_role = array( "SiteManager" 	=> "Gestionnairie",
		   "SiteCollaborator" 	=> "Collaborateur",
		   "SiteContributor" 	=> "Contributeur",
		   "SiteConsumer" 	=> "Lecture"
		);

# pour modifier les CSS(couleurs) en fonction des rôles :
$tab_role_css = array(	"SiteManager" 	   => "for_role_comm for_gestion",
			"SiteCollaborator" => "for_role_comm for_collabo",
		  	"SiteContributor"  => "for_role_comm for_contrib",
		  	"SiteConsumer" 	   => "for_role_comm for_lecture"
		);


?>
