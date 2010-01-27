' AddDB
' Usage:
'       AddDB src dest

If WScript.Arguments.Count < 2 then
  WScript.StdErr.WriteLine("usage AddDB.vbs src dest")
  WScript.Quit(1)
End If

Const ForReading = 1, ForWriting = 2, ForAppending = 8
Set objFSO = CreateObject("Scripting.FileSystemObject")
If (Not(objFSO.FileExists(WScript.Arguments(0)))) Then
  WScript.StdErr.WriteLine("Can't open: " & WScript.Arguments(0))
  WScript.Quit(1)
End If

Set objSrc = objfso.OpenTextFile(WScript.Arguments(0), ForReading)
Set objDest = objFSO.CreateTextFile(WScript.Arguments(1), true)

found = False

Set re = new regexp
re.Pattern = "ex15$"
If (re.Test(WScript.Arguments(0))) Then
  objDest.writeline "#DATABASE ${TARGETDIR};examples\ex15.dat"
  found = True
End If
  
re.Pattern = "ex14$"
If (re.Test(WScript.Arguments(0))) Then
  objDest.writeline "#DATABASE ${TARGETDIR};wateq4f.dat"
  found = True
End If

If (found <> True) Then
  objDest.writeline "#DATABASE ${TARGETDIR};phreeqc.dat"
End If

Do Until objSrc.AtEndOfStream <> False
  strLine = objSrc.ReadLine
  objDest.writeline strline
Loop
objDest.Close
