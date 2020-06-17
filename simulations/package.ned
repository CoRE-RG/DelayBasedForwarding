package delaybasedforwarding.simulations;
@license(LGPL);

import delaybasedforwarding.DelayBasedForwarding.HelloWorld;

network HelloWorldNetwork
{
    submodules:
        HelloWorld: HelloWorld {
            @display("p=52,52");
        }
}
