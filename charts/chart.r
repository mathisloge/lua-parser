library(ggplot2)
library(zoo)
luarocks <- read.csv(file = 'luarocks.csv')
koreader <- read.csv(file = 'koreader.csv')
awesome <- read.csv(file = 'awesome.csv')
luakit <- read.csv(file = 'luakit.csv')
apisix <- read.csv(file = 'apisix.csv')
projectfile <- rbind(koreader, luarocks)
projectfile <- rbind(projectfile, awesome)
projectfile <- rbind(projectfile, luakit)
projectfile <- rbind(projectfile, apisix)
head(projectfile)

plot_clones_loc <- ggplot(projectfile, aes(x = loc, y = nclones)) + 
    geom_line(aes(y=nclones_avg))+
    labs(x = "Lines of Code", y = "Anzahl Klone")
plot_clones_loc$labels$colour <- "Projekt"
ggsave(plot_clones_loc, file="plot_clones_loc.pdf", dpi=600)


plot_seq_loc <- ggplot(projectfile, aes(x = loc, y = nsequences, fill=project)) + 
    geom_line(aes(color=project)) +
    labs(x = "Lines of Code", y = "Anzahl Sequenzen")
plot_seq_loc$labels$colour <- "Projekt"
ggsave(plot_seq_loc, file="plot_seq_loc.pdf", dpi=600)


plot_seq_clone <- ggplot(projectfile, aes(x = nclones, y = nsequences, fill=project)) + 
    geom_line(aes(color=project)) +
    labs(x = "Anzahl Klone", y = "Anzahl Sequenzen")
plot_seq_clone$labels$colour <- "Projekt"
ggsave(plot_seq_clone, file="plot_seq_clone.pdf", dpi=600)
