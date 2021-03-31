library(ggplot2)
koreader <- read.csv(file = 'mass_koreader_t1.csv')
luarocks <- read.csv(file = 'mass_luarocks_t1.csv')
apisix <- read.csv(file = 'mass_apisix_t1.csv')
projectfile <- rbind(koreader, luarocks)
projectfile <- rbind(projectfile, apisix)
head(projectfile)

plot_clones_loc <- ggplot(projectfile, aes(x = mass, y = occurrences, fill=project)) + 
    geom_line(aes(color=project)) +
    labs(x = "Masse", y = "Vorkommen")
plot_clones_loc$labels$colour <- "Projekt"
ggsave(plot_clones_loc, file="plot_mass_t1.pdf", dpi=600)
