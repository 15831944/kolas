<html>
<body>

<?php

// Include ********************************************
require ("dbConn.inc");
include "function.inc";

//echo(" <script>
//       window.alert('$output')
//       </script>");
       
       
    $arrData ;
    $nCount = 0;   
	$string = $output;
	$token  = strtok($string, "[,]");
	while ($token !== false) {
	  //echo("$token<br>");
	  
	  $arrData[$nCount] = $token ;
	  
	  $token = strtok("[,]");
	  $nCount ++ ;
	  
	  //
	  
	}

	//GLOBAL $research_key;
	//$research_key = $arrData[0] ;
	
	
// oci class �ν��Ͻ� *********************************
	if (class_exists(DBCONN)) {
		$oci = new DBCONN();
	}
	else {
		MessageAlertPage(" DBCONN class �ʱ�ȭ ���� ");
		exit;
	}
// **************************************************************************
	
// query ����� *************************************************************	
	$nQueryCnt = 0 ;
	if($nCount > 1) 
	{
		for ($i = 1 ; $i < $nCount ; $i++)
		{
			$arrQuery[$i-1] = make_query($oci,$arrData[0],$arrData[$i]) ;		
			//echo("<p><p>arrQuery : $arrQuery[$i]") ;
			$nQueryCnt++ ;
		}
	}


// **************************************************************************       
       
// **************************************************************************       
// query ���� �ϱ�        
	
	for ($k = 0 ;$k < $nQueryCnt ; $k++)
	{
		//echo("<p><p>arrQuery : $arrQuery[$k]") ;
		if (!$oci->insert($arrQuery[$k])) {
			//MessagePage("$qry");
			//MessageAlertPage(" CIP ���� DB Insert ���� ");
			echo(" <script>
       		window.alert('DB Insert ����')
       		</script>");
			exit(1);
		}
		
		//echo("<p><p>arrQuery : $arrQuery[$k]") ;
	}

	$oci->commit();
    $oci->parseFree();
       
       
// **************************************************************************       
       

// **************************************************************************       
// Disconnect         
       
// **************************************************************************              
	echo("<p><p><p>���� ����� ���������� ó�� �Ǿ����ϴ�.") ;       
    echo("<p>������ �����Ͽ� �ּż� ���� �մϴ�..") ;          
php?>

</body>
</html>