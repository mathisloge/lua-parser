library(ggplot2)
luarocks <- read.csv(file = 'mass_luarocks.csv')
koreader <- read.csv(file = 'mass_koreader.csv')
awesome <- read.csv(file = 'mass_awesome.csv')
luakit <- read.csv(file = 'mass_luakit.csv')
apisix <- read.csv(file = 'mass_apisix.csv')
projectfile <- rbind(koreader, luarocks)
projectfile <- rbind(projectfile, awesome)
projectfile <- rbind(projectfile, luakit)
projectfile <- rbind(projectfile, apisix)
head(projectfile)

plot_clones_loc <- ggplot(projectfile, aes(x = mass, y = occurrences, fill=project)) + 
    geom_line(aes(color=project)) +
    labs(x = "Masse", y = "Vorkommen")
plot_clones_loc$labels$colour <- "Projekt"
ggsave(plot_clones_loc, file="plot_mass_t2.pdf", dpi=600)
