#import "hellocricketAppDelegate.h"
#import "ck/objc/ck.h"
#import "ck/objc/config.h"
#import "ck/objc/bank.h"
#import "ck/objc/sound.h"

@implementation hellocricketAppDelegate

@synthesize window;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    CkConfig config;
    CkConfigInit(&config);
    CkInit(&config);

    CkoBank* bank = [CkoBank newBank:@"hellocricket.ckb"];
    CkoSound* sound = [CkoSound newBankSound:bank index:0];
    [sound play];

    while (sound.playing)
    {
        CkUpdate();
        usleep(30000);
    }

    [sound release];
    [bank release];

    CkShutdown();
}

@end
