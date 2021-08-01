{
  "targets": [
    {
      "target_name": "node_dzcot",
      "type": "loadable_module",
      "dependencies": [
        "DzCot"
      ],
      "sources": [
        "src/wrapper.cc"
      ],
      "include_dirs": [
        "src/DzCot/include"
      ],
      "libraries": [
        "-lDzCot",
        "-L<(PRODUCT_DIR)/DzCot"
      ]
    },
    {
      "conditions": [
        [
          "OS=='linux'",
          {
            "target_name": "DzCot",
            "type": "none",
            "actions": [
              {
                "action_name": "build_DzCot",
                "inputs": [],
                "outputs": [
                  "DzCot/libDzCot.so"
                ],
                "action": [
                  "make",
                  "INSTALL_DIR=<(PRODUCT_DIR)",
                  "-C",
                  "./src/DzCot/src/DzCot/",
                  "install_static"
                ]
              }
            ]
          }
        ]
      ]
    }
  ]
}