\ProvidesClass{report}
\LoadClass{article}

\RequirePackage[T1]{fontenc}
\RequirePackage{lmodern}
\renewcommand*\familydefault{\sfdefault}
\RequirePackage{sfmath}
\RequirePackage{microtype}
\RequirePackage[utf8]{inputenc}

\usepackage{cfr-lm}

\RequirePackage{listings}
\lstset{language=java}
\RequirePackage{csquotes}
\RequirePackage[ngerman]{babel}
\RequirePackage[margin=2cm,a4paper]{geometry}

\RequirePackage{graphicx}
\graphicspath{{./}{../}{images/}}

%\RequirePackage[pdftex,
	%unicode=true,
	%breaklinks=true,
	%colorlinks=true,
	%linkcolor=black,
	%citecolor=black,
	%urlcolor=black]{hyperref}

\renewcommand{\baselinestretch}{1.1}
\setlength{\parindent}{0pt}
%\setlength{\parskip}{5pt}

\newcommand{\titleabstand}{\hspace{3cm}}
\newcommand{\RNum}[1]{\uppercase\expandafter{\romannumeral #1\relax}}

\renewcommand{\maketitle}{
\begin{titlepage}
	\thispagestyle{empty}
  \begin{minipage}[t]{1.0\textwidth}
	\includegraphics[scale=0.3]{images/logo_ub.png}
  \end{minipage}

 \begin{minipage}{\textwidth}
  \ \\
  \vspace{0.5cm}

  \begin{minipage}[t]{1.0\textwidth}
  	\begin{flushright}
  	\Large{Software-Reengineering}\\
	\Huge{Klonerkennung nach Baxter\\für Klone des Typs 1 und 2}\\
	\Large WS 20/21
  	\end{flushright}
    \end{minipage}
  %\hspace{1.8cm}
\end{minipage}

\vspace{17cm}

\Large
\begin{flushright}
Mathis Logemann\\
Master Informatik\\
Matr. 000000
\end{flushright}

\vspace{-0.9cm}
\begin{flushleft}
\href{mailto:malogema@uni-bremen.de}{malogema@uni-bremen.de}
\end{flushleft}

\end{titlepage}
}
