library(ggplot2)
luarocks <- read.csv(file = 'luarocks_t1.csv')
koreader <- read.csv(file = 'koreader_t1.csv')
awesome <- read.csv(file = 'awesome_t1.csv')
luakit <- read.csv(file = 'luakit_t1.csv')
apisix <- read.csv(file = 'apisix_t1.csv')
projectfile <- rbind(koreader, luarocks)
projectfile <- rbind(projectfile, awesome)
projectfile <- rbind(projectfile, luakit)
projectfile <- rbind(projectfile, apisix)
head(projectfile)

plot_clones_loc <- ggplot(projectfile, aes(x = loc, y = nclones, fill=project)) + 
    geom_line(aes(color=project)) +
    ggtitle("Typ-1: Lines of Code im Vergleich zu Klonen") + 
    labs(x = "Lines of Code", y = "Anzahl Klone")
plot_clones_loc$labels$colour <- "Projekt"
ggsave(plot_clones_loc, file="plot_clones_loc_t1.pdf", dpi=600)


plot_seq_loc <- ggplot(projectfile, aes(x = loc, y = nsequences, fill=project)) + 
    geom_line(aes(color=project)) +
    ggtitle("Typ-1: Lines of Code im Vergleich zu Klonen") + 
    labs(x = "Lines of Code", y = "Anzahl Sequenzen")
plot_seq_loc$labels$colour <- "Projekt"
ggsave(plot_seq_loc, file="plot_seq_loc_t1.pdf", dpi=600)


plot_seq_clone <- ggplot(projectfile, aes(x = nclones, y = nsequences, fill=project)) + 
    geom_line(aes(color=project)) +
    ggtitle("Typ-1: Anzahl Klone in relation zu der Anzahl an Sequenzen") + 
    labs(x = "Anzahl Klone", y = "Anzahl Sequenzen")
plot_seq_clone$labels$colour <- "Projekt"
ggsave(plot_seq_clone, file="plot_seq_clone_t1.pdf", dpi=600)
