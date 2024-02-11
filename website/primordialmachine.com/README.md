# Build and deploy
`build.ps1` updates this folder with the information from the `libraries` directory.
If `build.ps1` succeeds, this whole folder can be uploaded to a webhost.
Note that you must adjust the domain name in `config.php`.

# Updating MathJax
The website uses MathJax.
The library is shipped with the website.
The folder `static/external/mathjax` contains the MathJax library.

You can update to a more recent versions of MathJax library by replacing the contents of this folder
with the contents of `https://github.com/mathjax/MathJax/tree/master/es5`.

# Updating Google Fonts
The website uses Google Fonts web fonts.
The web fonts fonts are shipped with the website.
The folder `static/external/fonts` contains the Google Fonts webfonts.

You can update to a more recent version of the Google Fonts web fonts as follows:
- Clear the contents of `static/external/fonts`.
- Browse to `https://fonts.google.com/share?selection.family=Gelasio:ital,wght@0,400;0,500;0,600;0,700;1,400;1,500;1,600;1,700|Inconsolata:wdth,wght@50..200,200..900|Noto+Sans:ital,wght@0,100..900;1,100..900`.
- Select download and save `Gelasio,Inconsolata,Noto_Sans.zip` to `static/external/fonts`.
- Extract the contents of `static/external/fonts/Gelasio,Inconsolata,Noto_Sans.zip` to `static/external/fonts`.
