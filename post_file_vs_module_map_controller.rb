module Api
  class PostFileVsModuleMapController < ApplicationController
    require 'prototype/mod_get_impact_module_from_file.rb'
    require 'prototype/mod_misc_sheet_io.rb'
    #require 'memprof2'
    require "open3"
    include PostRelationMapHelper

    # deactive CSRF
    skip_before_action :verify_authenticity_token

    # curl -X POST http://host:port/api/post_file_module_map -F "file=file_path; type=application/excel"

    def do_regist_db
      # READ=========================================================================================
      #system('echo before read csv or excel >> /home/vagrant/Uroboros-O/test_log_1.txt')
      #system('top -b -n 1 >> /home/vagrant/Uroboros-O/test_log_1.txt')
      # Memprof2.start
      # Copy tmp file to original file
      excel_file_path = copy_tmp_to_other_file(params["file"])
      
      # for get excel headers
      file_vs_module_map = FileVsModuleMap.new(excel_file_path)

      misc_stats_hash = MiscSheetRW.get_misc_stats(excel_file_path)
      all_module_subteams = file_vs_module_map.total_module_sub_teams
      all_responsible_domains = file_vs_module_map.total_responsible_domains
      all_modfiles = file_vs_module_map.get_all_uniq_files
      all_dirfilepaths = file_vs_module_map.get_all_uniq_components

      create_target_array = [
                              [all_module_subteams, ModuleSubTeam, ModuleSubTeam::MODULE_SUB_TEAM_LABEL_COLUMN],
                              [all_responsible_domains, ResponsibleDomain, ResponsibleDomain::RESPONSIBLE_DOMAIN_LABEL_COUMN],
                              [all_modfiles, ModFile, ModFile::MOD_FILE_NAME_COLUMN],
                              [all_dirfilepaths, DirFilePath, DirFilePath::DIR_FILE_PATH_COLUMN]
                            ]

      create_target_array.each do |all_values, target_model, target_column|
        # create target Records
        result = create_records(all_values, target_model, target_column)
        return render json: result if Common::ErrorUtility.is_error?(result)
      end

      #system('echo after read csv or excel >> /home/vagrant/Uroboros-O/test_log_1.txt')
      #system('top -b -n 1 >> /home/vagrant/Uroboros-O/test_log_1.txt')
      #return render json:{ "read" => "Read success" }
      # Compare=========================================================================================
      system('echo before read csv or excel >> /home/vagrant/Uroboros-O/test_log_2.txt')
      system('top -b -n 1 >> /home/vagrant/Uroboros-O/test_log_2.txt')
      
      modfile_module_subteam_records = []
      modfile_responsible_domain_records = []
      modfile_unknown_commit_records = []

      # get Modfile relation records
      all_modfiles.each do |modfile|

        mod_file_id = ModFile.find_by(mod_file_name: modfile).id

        module_vs_rel_amount_stats = file_vs_module_map.get_impact_module_vs_rel_amount_map_from(modfile)

        if module_vs_rel_amount_stats.present?
          module_vs_rel_amount_stats.each do |mod, relation_amount|

            module_sub_team_id = ModuleSubTeam.find_by(module_sub_team_label: mod).id

            modfile_module_subteam_records << \
              ModFileModuleSubTeam.new( manifest_branch:      misc_stats_hash[MiscSheetRW::KEY_MANIFEST_BRANCH],
                                        manifest_tag:         misc_stats_hash[MiscSheetRW::KEY_MANIFEST_TAG],
                                        commit_date:          misc_stats_hash[MiscSheetRW::KEY_COMMIT_DATE],
                                        sha1:                 misc_stats_hash[MiscSheetRW::KEY_SHA1],
                                        mod_file_id:          mod_file_id,
                                        module_sub_team_id:   module_sub_team_id,
                                        relation_amount:      relation_amount )

          end
        end

        domain_vs_rel_amount_stats = file_vs_module_map.get_impact_domain_vs_rel_amount_map_from(modfile)

        if domain_vs_rel_amount_stats.present?
          domain_vs_rel_amount_stats.each do |domain, relation_amount|

            responsible_domain_id = ResponsibleDomain.find_by(responsible_domain_label: domain).id

            modfile_responsible_domain_records << \
              ModFileResponsibleDomain.new( manifest_branch:         misc_stats_hash[MiscSheetRW::KEY_MANIFEST_BRANCH],
                                            manifest_tag:            misc_stats_hash[MiscSheetRW::KEY_MANIFEST_TAG],
                                            commit_date:             misc_stats_hash[MiscSheetRW::KEY_COMMIT_DATE],
                                            sha1:                    misc_stats_hash[MiscSheetRW::KEY_SHA1],
                                            mod_file_id:             mod_file_id,
                                            responsible_domain_id:   responsible_domain_id,
                                            relation_amount:         relation_amount )

          end
        end

        unknown_commit_vs_rel_amount_stats = file_vs_module_map.get_impact_unknown_commit_vs_rel_amount_map_from(modfile)

        if unknown_commit_vs_rel_amount_stats.present?
          unknown_commit_vs_rel_amount_stats.each do |unknown_header, relation_amount|

            modfile_unknown_commit_records << \
              ModFileUnknownCommit.new( manifest_branch:         misc_stats_hash[MiscSheetRW::KEY_MANIFEST_BRANCH],
                                        manifest_tag:            misc_stats_hash[MiscSheetRW::KEY_MANIFEST_TAG],
                                        commit_date:             misc_stats_hash[MiscSheetRW::KEY_COMMIT_DATE],
                                        sha1:                    misc_stats_hash[MiscSheetRW::KEY_SHA1],
                                        mod_file_id:             mod_file_id,
                                        relation_amount:         relation_amount )

          end
        end

      end # all_modfiles.each


      # get Dirfilepath relation records
      dirfilepath_module_subteam_records = []
      dirfilepath_responsible_domain_records = []
      dirfilepath_unknown_commit_records = []

      all_dirfilepaths.each do |dir_file_path|

        dir_file_path_id = DirFilePath.find_by(dir_file_path: dir_file_path).id

        module_vs_rel_amount_stats = file_vs_module_map.get_impact_module_vs_rel_amount_map_from_component(dir_file_path)

        if module_vs_rel_amount_stats.present?
          module_vs_rel_amount_stats.each do |mod, relation_amount|

            module_sub_team_id = ModuleSubTeam.find_by(module_sub_team_label: mod).id

            dirfilepath_module_subteam_records << \
              DirFilePathModuleSubTeam.new( manifest_branch:      misc_stats_hash[MiscSheetRW::KEY_MANIFEST_BRANCH],
                                            manifest_tag:         misc_stats_hash[MiscSheetRW::KEY_MANIFEST_TAG],
                                            commit_date:          misc_stats_hash[MiscSheetRW::KEY_COMMIT_DATE],
                                            sha1:                 misc_stats_hash[MiscSheetRW::KEY_SHA1],
                                            dir_file_path_id:     dir_file_path_id,
                                            module_sub_team_id:   module_sub_team_id,
                                            relation_amount:      relation_amount )

          end
        end

        domain_vs_rel_amount_stats = file_vs_module_map.get_impact_domain_vs_rel_amount_map_from_component(dir_file_path)

        if domain_vs_rel_amount_stats.present?
          domain_vs_rel_amount_stats.each do |domain, relation_amount|

            responsible_domain_id = ResponsibleDomain.find_by(responsible_domain_label: domain).id

            dirfilepath_responsible_domain_records << \
              DirFilePathResponsibleDomain.new( manifest_branch:        misc_stats_hash[MiscSheetRW::KEY_MANIFEST_BRANCH],
                                                manifest_tag:           misc_stats_hash[MiscSheetRW::KEY_MANIFEST_TAG],
                                                commit_date:            misc_stats_hash[MiscSheetRW::KEY_COMMIT_DATE],
                                                sha1:                   misc_stats_hash[MiscSheetRW::KEY_SHA1],
                                                dir_file_path_id:       dir_file_path_id,
                                                responsible_domain_id:  responsible_domain_id,
                                                relation_amount:        relation_amount )

          end
        end

        unknown_commit_vs_rel_amount_stats = file_vs_module_map.get_impact_unknown_commit_vs_rel_amount_map_from_component(dir_file_path)

        if unknown_commit_vs_rel_amount_stats.present?
          unknown_commit_vs_rel_amount_stats.each do |unknown_header, relation_amount|
            dirfilepath_unknown_commit_records << \
              DirFilePathUnknownCommit.new( manifest_branch:        misc_stats_hash[MiscSheetRW::KEY_MANIFEST_BRANCH],
                                            manifest_tag:           misc_stats_hash[MiscSheetRW::KEY_MANIFEST_TAG],
                                            commit_date:            misc_stats_hash[MiscSheetRW::KEY_COMMIT_DATE],
                                            sha1:                   misc_stats_hash[MiscSheetRW::KEY_SHA1],
                                            dir_file_path_id:       dir_file_path_id,
                                            relation_amount:        relation_amount )

          end
        end

      end # all_dirfilepaths.each

      system('echo after read csv or excel >> /home/vagrant/Uroboros-O/test_log_2.txt')
      system('top -b -n 1 >> /home/vagrant/Uroboros-O/test_log_2.txt')
      return render json:{ "Compare" => "Compare success" }

      # WRITE========================================================================================
      #system('echo before read csv or excel >> /home/vagrant/Uroboros-O/test_log_3.txt')
      #system('top -b -n 1 >> /home/vagrant/Uroboros-O/test_log_3.txt')
      relation_record_array = [
                               [ModFileModuleSubTeam, modfile_module_subteam_records],
                               [ModFileResponsibleDomain, modfile_responsible_domain_records],
                               [ModFileUnknownCommit, modfile_unknown_commit_records],
                               [DirFilePathModuleSubTeam, dirfilepath_module_subteam_records],
                               [DirFilePathResponsibleDomain, dirfilepath_responsible_domain_records],
                               [DirFilePathUnknownCommit, dirfilepath_unknown_commit_records]
                              ]

      relation_record_array.each do |target_model, target_record|
        target_model.where(sha1: misc_stats_hash[MiscSheetRW::KEY_SHA1]).delete_all
        # save target relation Records
        result = save_record(target_record)
        return render json: result if Common::ErrorUtility.is_error?(result)
      end
      #system('echo after read csv or excel >> /home/vagrant/Uroboros-O/test_log_3.txt')
      #system('top -b -n 1 >> /home/vagrant/Uroboros-O/test_log_3.txt')
      return render json:{ "result" => "regist success" }
      #Memprof2.report(out: "/tmp/memprof2_log")
      #Memprof2.report
      #Memprof2.stop
    end

  end

end
