### XChat plugin: makes a colorful text with: /rb <text>
# Originally by Axioplase
# Fixed by Phr33d0m: it randomizes the colors on each line now
#                    and it doesn't eat numbers <10 anymore.

IRC::register("hexchat rainbow", "0.1", "", "");

sub rainbow{
    $_=shift@_;
    chomp;

    s{(.)}{
	"\cC" . (int(rand(14))+2) . "\cB\cB$1"
    }eg;

    IRC::command ("/say $_");
    return 1;
}


IRC::add_command_handler("rb", "rainbow");
IRC::print('Loaded rainbow Script by Axioplase and Phr33d0m.');
