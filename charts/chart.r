library(ggplot2)
luarocks <- read.csv(file = 'luarocks.csv')
second <- read.csv(file = 'second.csv')
projectfile <- rbind(second, luarocks)
head(projectfile)

plot_clones_loc <- ggplot(projectfile, aes(x = loc, y = nclones, fill=project)) + 
    geom_line(aes(color=project)) +
    ggtitle("Lines of Code im Vergleich zu Klonen") + 
    labs(x = "Lines of Code", y = "Anzahl Klone")
plot_clones_loc$labels$colour <- "Projekt"
ggsave(plot_clones_loc, file="plot_clones_loc.pdf", dpi=600)


plot_seq_loc <- ggplot(projectfile, aes(x = loc, y = nsequences, fill=project)) + 
    geom_line(aes(color=project)) +
    ggtitle("Lines of Code im Vergleich zu Klonen") + 
    labs(x = "Lines of Code", y = "Anzahl Klone")
plot_seq_loc$labels$colour <- "Projekt"
ggsave(plot_seq_loc, file="plot_seq_loc.pdf", dpi=600)


plot_seq_clone <- ggplot(projectfile, aes(x = nclones, y = nsequences, fill=project)) + 
    geom_line(aes(color=project)) +
    ggtitle("Anzahl Klone in relation zu der Anzahl an Sequenzen") + 
    labs(x = "Anzahl Klone", y = "Anzahl Sequenzen")
plot_seq_clone$labels$colour <- "Projekt"
ggsave(plot_seq_clone, file="plot_seq_clone.pdf", dpi=600)
