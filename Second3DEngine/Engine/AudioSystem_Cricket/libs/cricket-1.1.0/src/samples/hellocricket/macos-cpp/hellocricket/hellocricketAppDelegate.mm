#import "hellocricketAppDelegate.h"
#import "ck/ck.h"
#import "ck/config.h"
#import "ck/bank.h"
#import "ck/sound.h"

@implementation hellocricketAppDelegate

@synthesize window;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    CkConfig config;
    CkInit(&config);

    CkBank* bank = CkBank::newBank("hellocricket.ckb");
    CkSound* sound = CkSound::newBankSound(bank, 0);
    sound->play();

    while (sound->isPlaying())
    {
        CkUpdate();
        usleep(30000);
    }

    sound->destroy();
    bank->destroy();

    CkShutdown();
}

@end
