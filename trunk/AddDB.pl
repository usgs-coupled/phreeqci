# AddDB
# Usage:
#       AddDB src dest

open(SRC, "$ARGV[0]") || die "Can't open $ARGV[0]\n";
open(DEST, ">$ARGV[1]") || die "Can't open $ARGV[1]\n";


# check exceptions
if ($ARGV[0] =~ /ex15$/)
{
	print DEST "#DATABASE \$\{TARGETDIR\};examples\\ex15.dat\n";	
}
elsif ($ARGV[0] =~ /ex14$/)
{
	print DEST "#DATABASE \$\{TARGETDIR\};wateq4f.dat\n";	
}
else
{
	print DEST "#DATABASE \$\{TARGETDIR\};phreeqc.dat\n";	
}

while (<SRC>)
{
	print DEST;
}
close(SRC);
close(DEST);
