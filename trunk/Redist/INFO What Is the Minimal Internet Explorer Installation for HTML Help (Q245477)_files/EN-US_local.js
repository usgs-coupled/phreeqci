var ToolBar_Supported = ToolBar_Supported ;
if (ToolBar_Supported != null && ToolBar_Supported == true)
{
	//To Turn on/off Frame support, set Frame_Supported = true/false.
	/* Code added below to test to see if this should be treated as being in a frameset or not (a-peterh)*/
	strQueryString = document.location.search;
	strQueryString = strQueryString.toUpperCase();
	if (strQueryString.indexOf("?FR=1") != -1 || strQueryString.indexOf("&FR=1") != -1)
		Frame_Supported = true;
	else
		Frame_Supported = false;


	// Customize default ICP menu color - bgColor, fontColor, mouseoverColor
	setDefaultICPMenuColor("#000000", "white", "red");

	// Customize toolbar background color
	setToolbarBGColor("white");

	// display ICP Banner
	setICPBanner("/isapi/gosupport.asp?target=/library/images/support/pss_EN-US.gif","/isapi/gosupport.asp?target=/directory/default.asp","Microsoft Product Support Services") ;
	
	//***** Add ICP menus *****
	//Home
	addICPMenu("HomeMenu", "Home", "Home","/isapi/gosupport.asp?target=/directory/default.asp");
	addICPSubMenu("HomeMenu","Support&nbsp;Home","/isapi/gosupport.asp?target=/directory/default.asp");
	addICPSubMenu("HomeMenu","microsoft.com","/isapi/gomscom.asp?target=/");

	//Self Support
	addICPMenu("SelfSuppMenu", "Find a Solution", "Find a Solution","/isapi/gosupport.asp?target=/directory/self.asp?SD=GN&amp;fr=0");
	setICPSubMenuWidth("SelfSuppMenu","relative","0.7");	
	addICPSubMenu("SelfSuppMenu","Search&nbsp;Knowledge&nbsp;Base","/isapi/gokbsearch.asp?target=/kb/c.asp?ln=en-us&sd=gn");
	addICPSubMenu("SelfSuppMenu","Search&nbsp;Wizard","/isapi/gosupport.asp?target=/Directory/search_wizard/searchwizard.asp?ln=en-us");
	addICPSubMenu("SelfSuppMenu","FAQs&nbsp;by&nbsp;Product","/isapi/gosupport.asp?target=/directory/faqs.asp?SD=GN&amp;fr=0"); 
	addICPSubMenu("SelfSuppMenu","Find&nbsp;Software&nbsp;Downloads","/isapi/gomscom.asp?target=/downloads/search.asp");
	addICPSubMenu("SelfSuppMenu","Newsgroups","http://communities.microsoft.com/newsgroups/default.asp?icp=GSS&slcid=us");

	//Assisted Support
	addICPMenu("AssistMenu", "Request&nbsp;Support", "Request Support","/isapi/gosupport.asp?target=/directory/directory.asp?ln=en-us&sd=gn&amp;fr=0");
	addICPSubMenu("AssistMenu","Get&nbsp;Help&nbsp;From&nbsp;Microsoft","/isapi/gosupport.asp?target=/directory/getitem.asp?item=OS&sd=gn&amp;fr=0");
	addICPSubMenu("AssistMenu","Check&nbsp;Status&nbsp;of&nbsp;Your&nbsp;Pending&nbsp;Question","https://WebResponse.one.microsoft.com/wrscripts/en/srlist.asp");
	addICPSubMenu("AssistMenu","Call&nbsp;Microsoft&nbsp;Support","/isapi/gosupport.asp?target=/directory/getitem.asp?item=PH&SD=GN&amp;fr=0");

	//Custom Support
	addICPMenu("CustomMenu", "Custom&nbsp;Support","Comprehensive&nbsp;support&nbsp;customized&nbsp;by&nbsp;customer&nbsp;type","/isapi/gosupport.asp?target=/directory/customer.asp?Sd=gn&amp;fr=0");
	//setICPSubMenuWidth("CustomMenu","relative","0.9");
	//addICPSubMenu("CustomMenu","Developers","/isapi/gosupport.asp?target=/servicedesks/msdn/default.asp");
	//addICPSubMenu("CustomMenu","IT&nbsp;Professionals","/isapi/gosupport.asp?target=/technet/support/default.asp");	//addICPSubMenu("CustomMenu","Resellers&nbsp;&amp;&nbsp;Consultants","/isapi/gosupport.asp?target=/servicedesks/directaccess/default.htm");
	//addICPSubMenuLine("CustomMenu");
	//addICPSubMenu("CustomMenu","MCSP","https://mcsp.microsoft.com/support");	
	//addICPSubMenu("CustomMenu","Computer&nbsp;Manufacturers","https://www.msbpn.com/oem/");	
	//addICPSubMenu("CustomMenu","Premier","https://servicedesk.one.microsoft.com/premier/");	


}




