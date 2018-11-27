set EFStartAddr 0x10000000
set EFEndAddr   0x1003FFFF


set NULL_CHAR [format %c 255]

set curTCLpath  [file dirname [info nameofexecutable]]

set stsError 0

# ------------------------------------------------ #
#     get binary file and address information      #
# ------------------------------------------------ #
if {$argc >= 1} { 
    set ICFORM [lindex $argv 0]
    
    
    # get bin_list from argv
    #set bin_list [lrange $argv 1 end]
    set bin_list {}
    foreach {binName binAdr} [lrange $argv 1 end] {
        #set locallist "[file nativename $binName] $binAdr"
        set locallist "[string cat \" [file dirname $binName] / [file tail $binName] \"] $binAdr"

        if {[file exist [lindex $locallist 0]]!=1} {
            puts stdout "file:\"$binName\" is not exist"
            set stsError 1
        }

        if { [string is integer -strict $binAdr]!=1 || $binAdr<$EFStartAddr || $binAdr>$EFEndAddr} {
            puts stdout "No valid $ICFORM eflash address for $binName"
            set stsError 1
        }
        
        if {$stsError==0} {
            lappend bin_list $locallist
        }
    }
    # remap bin_list by integer
    set bin_list [lsort -integer -index 1 $bin_list]
    set bin_list_Cnt [llength $bin_list]
    
} else {
    puts stdout "USAGE: $argv0 ICFORM bin1.bin addr1 bin2.bin addr2 bin3.bin addr3..."
    set stsError 1
}

if {$stsError} {
    exit
}


# ------------------------------------------------ #
#     check j-link device is exist                 #
# ------------------------------------------------ #
#catch {exec devcon.exe find =USB } LogTxt
#puts stdout $LogTxt

#set ScriptFailStr "J-Link"
#if {[string first  $ScriptFailStr $LogTxt]<0} {
#    puts stdout "No J-link Device found."
#    exit
#}


# ------------------------------------------------ #
#     merge all binary file                        #
# ------------------------------------------------ #
set FileMerge "$curTCLpath/tmpbin.bin"
set FileStartAdr [lindex [lindex $bin_list 0] 1] 
file delete $FileMerge


set FMergeID [open $FileMerge a+]
fconfigure $FMergeID -translation binary

for {set x 0} {$x<$bin_list_Cnt} {incr x} {
    #  retrieve binfile name and start address
    set ThisBinName [lindex [lindex $bin_list [expr $x+0]] 0]
    set ThisBinAdr  [lindex [lindex $bin_list [expr $x+0]] 1]
    set ThisBinSize [file size $ThisBinName]

    set NextBinAdr  [lindex [lindex $bin_list [expr $x+1]] 1]

    if {[expr $x+1] < $bin_list_Cnt} {
        set binarySize [expr  $NextBinAdr - $ThisBinAdr - $ThisBinSize]
    } else {
        set binarySize 0
    }

    #  process source file
    set FSource [open $ThisBinName r]
    fconfigure $FSource -translation binary
    puts -nonewline $FMergeID [read $FSource $ThisBinSize]
    close $FSource


    for {set fp 0} {$fp<$binarySize} {incr fp 1} {
        puts -nonewline $FMergeID $NULL_CHAR
    }
}

close $FMergeID


# ------------------------------------------------ #
#     prepare the filepath for jlink.exe           #
# ------------------------------------------------ #
#set fileCmd $curTCLpath/cmdscript.jlink
#set Jlinkexe $curTCLpath/jlink.exe
#set JlinkSetting $curTCLpath/cc6801_DL.jlinksettings
#set LogFile $curTCLpath/Log.txt
set fileCmd [string cat $curTCLpath / cmdscript.jlink]
set Jlinkexe [string cat $curTCLpath / jlink.exe]
set JlinkSetting [string cat $curTCLpath / cc6801_DL.jlinksettings]
#set LogFile [string cat $curTCLpath / Log.txt]



# ------------------------------------------------ #
#     prepare the jlink script                     #
# ------------------------------------------------ #
set FIDcmd [open $fileCmd w+]
puts $FIDcmd "si swd"
puts $FIDcmd "speed 4000"
puts $FIDcmd "loadfile \"$FileMerge\" $FileStartAdr"
puts $FIDcmd "q"
close $FIDcmd


# ------------------------------------------------ #
#     execute eflash program                       #
# ------------------------------------------------ #

#exec $Jlinkexe -device $ICFORM -settingsfile $JlinkSetting -CommanderScript $fileCmd > $LogFile
catch {exec $Jlinkexe -device $ICFORM -settingsfile $JlinkSetting -CommanderScript $fileCmd } LogTxt

puts stdout $LogTxt 

# ------------------------------------------------ #
#     end program, delete dummy files and output   #
#     complete message                             #
# ------------------------------------------------ #
set ScriptDoneStr "J-Link: Flash download: Total time needed:"
if {[string first  $ScriptDoneStr $LogTxt]>=0} {
    puts stdout "Flash download: completed."
} else {
    puts stdout "Flash download Failed."
}
file delete $FileMerge

exit 1


