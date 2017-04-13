# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure("2") do |config|
  config.vm.box = "bento/ubuntu-16.04"
  config.vm.hostname = "videostream-dev"
  config.vm.network "private_network", ip: "172.28.128.42"

  config.vm.provider "virtualbox" do |vb|
    vb.gui = true
    vb.memory = "1024"
    vb.name = "videostream-dev"
  end

  config.vm.provision "ansible_local" do |ansible|
    ansible.playbook = "create-vm.yml"
  end
end
