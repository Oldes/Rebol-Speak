Rebol [
    title: "Rebol/Speak extension CI test"
]

print ["Running test on Rebol build:" mold to-block system/build]

system/options/quiet: false
system/options/log/rebol: 4

if CI?: any [
    "true" = get-env "CI"
    "true" = get-env "GITHUB_ACTIONS"
    "true" = get-env "TRAVIS"
    "true" = get-env "CIRCLECI"
    "true" = get-env "GITLAB_CI"
][
    ;; configure modules location for the CI test 
    system/options/modules: dirize to-rebol-file any [
        get-env 'REBOL_MODULES_DIR
        what-dir
    ]
    ;; make sure that we load a fresh extension
    try [system/modules/speak: none]
]

;; import the extension
tts: import 'speak

print as-yellow "Content of the module..."
? tts

tts/list-voices
