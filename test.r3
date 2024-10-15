Rebol [
	title: "Rebol/Speak extension test"
]

print ["Running test on Rebol build:" mold to-block system/build]

;; make sure that we load a fresh extension
try [system/modules/speak: none]

speak: import speak

? speak

speak/list-voices

;say system/license

;; do some test with the extension
say {
<emph> boo </emph>! boo!
<rate absspeed="5">
   This text should be spoken at rate five.
   <rate absspeed="-5">
      This text should be spoken at rate negative five.
   </rate>
</rate>
<rate absspeed="10"/>}
say "This sounds normal <pitch middle = '-10'/> but the pitch drops half way through"

eva: say/as/no-wait "<rate speed='2'/>" 2
say/as "How are you? <pitch middle='-10'/> How are you?<pitch absmiddle='+10'/>How are you?" :eva
say/as "<pitch absmiddle='-10'/>This is a good day." :eva
esp: say/as "Holla muchachos! Pobre amigos." 3

print "done"

