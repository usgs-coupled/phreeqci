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
	addICPMenu("HomeMenu", "Home", "Support Home","/isapi/gosupport.asp?target=/directory/default.asp");
	addICPSubMenu("HomeMenu","Support Home","/isapi/gosupport.asp?target=/directory/default.asp");
	addICPSubMenu("HomeMenu","microsoft.com","/isapi/gomscom.asp?target=/");

	//Self Support
	addICPMenu("SelfSuppMenu", "Self Support", "Self Support","/isapi/gosupport.asp?target=/directory/self.asp");
	addICPSubMenu("SelfSuppMenu","Searchable Knowledge Base","/isapi/gokbsearch.asp?target=/kb/c.asp");
	setICPSubMenuWidth("SelfSuppMenu","relative","0.7");
	addICPSubMenu("SelfSuppMenu","Download Center","/isapi/gomscom.asp?target=/downloads/search.asp");
	addICPSubMenu("SelfSuppMenu","FAQs by Product","/isapi/gosupport.asp?target=/directory/faqs.asp"); 

	//Assisted Support
	addICPMenu("AssistMenu", "Assisted Support", "Assisted Support","/isapi/gosupport.asp?target=/directory/getitem.asp?item=AD");
	addICPSubMenu("AssistMenu","Assisted Support Directory","/isapi/gosupport.asp?target=/directory/getitem.asp?item=AD");
	addICPSubMenu("AssistMenu","Online Support Requests","/isapi/gosupport.asp?target=/directory/getitem.asp?item=OS");
	addICPSubMenu("AssistMenu","Phone Numbers ","/isapi/gosupport.asp?target=/directory/getitem.asp?item=PH");

	//Custom Support
	addICPMenu("CustomMenu", "Custom Support","Comprehensive support customized by customer type","/isapi/gosupport.asp?target=/directory/getitem.asp?item=CS");
	//setICPSubMenuWidth("CustomMenu","relative","0.9");
	//addICPSubMenu("CustomMenu","Home Customers","/isapi/gosupport.asp?target=/support/default.asp");
	//addICPSubMenu("CustomMenu","Developers","/isapi/gosupport.asp?target=/servicedesks/msdn/default.htm");
	//addICPSubMenu("CustomMenu","IT Professionals","/isapi/gosupport.asp?target=/technet/support/default.htm");	
	//addICPSubMenu("CustomMenu","Resellers &amp; Consultants","/isapi/gosupport.asp?target=/servicedesks/directaccess/default.htm");
	//addICPSubMenuLine("CustomMenu");
	//addICPSubMenu("CustomMenu","MCSP","https://mcsp.microsoft.com/support");	
	//addICPSubMenu("CustomMenu","Computer Manufacturers","https://www.msbpn.com/oem/");	
	//addICPSubMenu("CustomMenu","Premier","https://servicedesk.one.microsoft.com/premier/");	

	//Worldwide Support
	addICPMenu("WWMenu","Worldwide Support","Support by worldwide location","/isapi/gosupport.asp?target=/directory/ww.asp");
	addICPSubMenu("WWMenu","Worldwide Support","/isapi/gosupport.asp?target=/directory/ww.asp");
	
	//Send us Your Feedback
	//addICPMenu("FeedbackMenu","Send us your feedback","Send us your feedback","/isapi/gosupport.asp?target=/directory/feedback/default.asp");
}




