# Copy the contents of
# "./../../libraries/data-definition-language/documentation/sources"
# to
# "./contents/data-definition-language"
Remove-Item "./contents/data-definition-language" -Force -Recurse -ErrorAction SilentlyContinue
Copy-Item "./../../libraries/data-definition-language/documentation/sources" -Destination "./contents/data-definition-language" -Recurse

# Copy the contents of
# "./../../libraries/asset-definition-language/documentation/sources"
# to
# "./contents/asset-definition-language"
Remove-Item "./contents/asset-definition-language" -Force -Recurse -ErrorAction SilentlyContinue
Copy-Item "./../../libraries/asset-definition-language/documentation/sources" -Destination "./contents/asset-definition-language" -Recurse

# Copy the contents of
# "./../../libraries/core/documentation/sources"
# to
# "./contents/core"
Remove-Item "./contents/core" -Force -Recurse -ErrorAction SilentlyContinue
Copy-Item "./../../libraries/core/documentation/sources" -Destination "./contents/core" -Recurse

# Copy the contents of
# "./../../libraries/assets/documentation/sources"
# to
# "./contents/assets"
Remove-Item "./contents/assets" -Force -Recurse -ErrorAction SilentlyContinue
Copy-Item "./../../libraries/assets/documentation/sources" -Destination "./contents/assets" -Recurse

# Copy the contents of
# "./../../libraries/aal/documentation/sources"
# to
# "./contents/aal"
Remove-Item "./contents/aal" -Force -Recurse -ErrorAction SilentlyContinue
Copy-Item "./../../libraries/aal/documentation/sources" -Destination "./contents/aal" -Recurse

# Copy the contents of
# "./../../libraries/aal/documentation/sources"
# to
# "./contents/val"
Remove-Item "./contents/val" -Force -Recurse -ErrorAction SilentlyContinue
Copy-Item "./../../libraries/val/documentation/sources" -Destination "./contents/val" -Recurse
