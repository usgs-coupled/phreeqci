window.onunload = KBSurvey;

function KBSurvey()
{
	var mnemonic = document.VOTED.KBAREA.value;
	var KBarticle = document.VOTED.KBID.value;

	var arrKB = mnemonic.split('; ');
	for(i=0; i<KBInfo.length; i++){
		if(Math.random()*KBInfo[i].obs<=1){
//			if(SitejsVal.toLowerCase() + '_' + LNjsVal.toLowerCase() + '_' + arrKB[2].toLowerCase()==KBInfo[i].id.toLowerCase()){
			if('kb_' + LNjsVal.toLowerCase() + '_' + arrKB[2].toLowerCase()==KBInfo[i].id.toLowerCase()){
				var cDate = new Date();
				if(null == GetCookie('KBDate')){cDate = null;}
				else{cDate = Date.parse(GetCookie('KBDate'));}
				var eDate = new Date();
				eDate = Date.parse(dateSub(KBInfo[i].cExpire));
				if((null == cDate) || (cDate <= eDate)){
					SetCookie('KBDate',new Date(),dateAdd(KBInfo[i].cExpire));
					URLsurvey = KBInfo[i].surveyURL + '&LN=' + LNjsVal + '&SD=' + SitejsVal + '&KB=' + KBarticle + '&purl=' + location.href;
					loadSurvey(URLsurvey,480,640);}}}}
}

function GetCookie(sName)
{
	var aCookie = document.cookie.split("; ");
	for (j=0; j<aCookie.length; j++){
		var aCrumb = aCookie[j].split("=");
		if(sName==aCrumb[0]){return unescape(aCrumb[1]);}}
	return null;
}

function SetCookie(sName,sValue,sExpires)
{
	document.cookie = sName + "=" + escape(sValue) + "; expires=" + sExpires + "; domain=" + ServerjsVal + ";"
}

function dateSub(sDays)
{
	var sToday = new Date();
	var millidiff = 1000*60*60*24*sDays;
	return new Date(Date.parse(sToday)-(millidiff));
}

function dateAdd(sDays)
{
	var sToday = new Date();
	var millidiff = 1000*60*60*24*sDays;
	return new Date(Date.parse(sToday)+(millidiff));
}

function loadSurvey(sPage,sHeight,sWidth)
{
	var sAttributes = 'directories=0,height='+sHeight+',location=1,resizable=1,scrollbars=1,status=0,toolbar=0,width='+sWidth;
	window.open(sPage, null, sAttributes);
}